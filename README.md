# Simulador de un Autómata con Pila

**Autor:** Marco Pérez Padilla  
**Correo:** alu0101469348@ull.edu.es  
**Asignatura:** Complejidad Computacional — Curso 2026/27  

Simulador orientado a objetos de un autómata con pila, escrito en C++. Dado
un fichero de texto con la definición formal del autómata y una serie de
cadenas de entrada, el programa indica si cada cadena pertenece al lenguaje
que reconoce el autómata, con la opción de mostrar paso a paso la traza de
la simulación.

## Índice

- [Introducción](#introducción)
- [Tipo de autómata implementado](#tipo-de-autómata-implementado)
- [Formato del fichero de definición](#formato-del-fichero-de-definición)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Diseño y decisiones técnicas](#diseño-y-decisiones-técnicas)
- [Manejo de errores](#manejo-de-errores)
- [Compilación y ejecución](#compilación-y-ejecución)
- [Tests](#tests)
- [Contenedor Docker](#contenedor-docker)
- [Repositorio](#repositorio)

## Introducción

Un autómata con pila (AP) es un reconocedor de lenguajes de tipo 2
(independientes del contexto) que, además de un control finito de estados,
dispone de una pila de memoria auxiliar. En esta práctica se pide programar
un simulador de AP con un diseño orientado a objetos, que lea la definición
del autómata desde un fichero de texto, valide que esa definición cumple la
definición formal de un AP y, a partir de ahí, permita comprobar si distintas
cadenas de entrada pertenecen al lenguaje reconocido.

El enunciado exige implementar **solo uno** de los dos criterios de
aceptación posibles (por vaciado de pila o por estado final). Por mi cuenta,
y como reto añadido, he decidido implementar **los dos**, indicando en cada
ejecución cuál le corresponde al fichero de definición (lo explico en la
siguiente sección).

El proyecto tiene una estructura de proyecto estándar con `include/`, `src/`, `tests/` y `data/`.

## Tipo de autómata implementado

Como decía, el programa soporta **ambos tipos**:

- **APv**: aceptación por vaciado de pila. La cadena se acepta si, tras
  consumirla entera, la pila queda vacía.
- **APf**: aceptación por estado final. La cadena se acepta si, tras
  consumirla entera, el autómata está en uno de los estados de `F`.

El tipo se indica con la opción obligatoria `-type apv|apf`; no se detecta
automáticamente a partir del contenido del fichero. El motivo es que el
formato de definición es ambiguo en un caso concreto: un APf cuyo conjunto
de estados finales `F` está vacío es indistinguible de un APv, porque la
línea de `F` desaparece del fichero al no tener ningún token, exactamente
igual que si esa línea nunca hubiese existido. Con `-type`, el programa ya
no necesita adivinar: lee la línea posterior al símbolo inicial de pila de
forma estricta según el tipo indicado (como conjunto de estados finales si
es `apf`, o como primera transición si es `apv`), y si el fichero no encaja
con el tipo pedido, falla con un error claro en vez de interpretarlo como
el otro tipo sin más.

## Formato del fichero de definición

El fichero de definición (el que se pasa con `-config`) sigue el formato
dado en el enunciado: comentarios con `#`, un elemento por línea, y epsilon
representado con un punto (`.`). Por ejemplo, así es uno de mis ficheros de
prueba para el lenguaje `L = {aⁿbⁿ | n > 0}` con aceptación por vaciado de
pila:

```txt
# Pushdown automaton with acceptance by empty stack, recognizing
#  L = {a^nb^n | n > 0}
# Epsilon is represented with a dot (.)
q1 q2
a b
S A
q1
S
q1 a S q1 A
q1 a A q1 AA
q1 b A q2 .
q2 b A q2 .
```

Un detalle del formato en el que merece la pena pararse: cuando el símbolo a
apilar es más de un carácter, como en `q1 a S q1 A`, el primer carácter de
esa cadena queda en la cima de la pila. Internamente, esto se traduce en
`nueva_pila = apilar + pila.substr(1)`, es decir, sustituir la cima antigua
por la cadena nueva de un plumazo.

## Estructura del proyecto

```
.
├── Makefile
├── README.md
├── include/            # cabeceras (.h), organizadas por responsabilidad
│   ├── exceptions/      # jerarquía de excepciones (exceptions.h)
│   ├── model/           # Alphabet, Transition, Configuration, PushdownAutomaton...
│   ├── acceptance/      # Strategy: AcceptanceCriterion y sus dos implementaciones, y su Factory
│   ├── simulation/      # Simulator (la búsqueda en profundidad)
│   ├── io/              # AutomatonParser, WordChecker, Menu, Application, TraceWriter, Options
│   └── help/            # Help, Usage, PrintWarning/PrintError, ValidateArguments
├── src/                # implementación (.cc), misma organización que include/
├── tests/              # tests de GoogleTest, un fichero por cada .h/.cc de arriba
├── libs/googletest/     # código fuente de GoogleTest
└── data/
    ├── automata/         # ficheros de definición (para -config), válidos y de error
    └── strings/          # ficheros de cadenas de entrada (para -in)
```

## Diseño y decisiones técnicas

- **Patrón Strategy** para el criterio de aceptación: `AcceptanceCriterion`
  es la interfaz, con `FinalStateAcceptance` y `EmptyStackAcceptance` como
  las dos estrategias concretas. 
- **Patrón Factory** (`MakeAcceptanceCriterion`): dado el tipo indicado por
  línea de comandos, construye la estrategia concreta correspondiente.
- **Simulación por DFS**: `Simulator::Explore` recorre las configuraciones
  del autómata en profundidad, probando antes las transiciones que
  consumen símbolo y después las de ε, con un conjunto de configuraciones
  visitadas para no entrar en bucles de transiciones ε y un límite de
  profundidad como red de seguridad frente a pilas que crecen sin límite.
- **`TraceWriter` separado del simulador**: el simulador nunca escribe
  directamente por pantalla; le pasa cada configuración a un `TraceWriter`,
  así que cambiar el formato de la traza no obliga a tocar la lógica de
  búsqueda.

## Manejo de errores

Distingo dos niveles de gravedad en todo el programa:

- **Errores críticos** (se reporta con `Error:` por `stderr` y el programa
  termina con código `1`): fichero de definición inexistente o vacío,
  definición que incumple la definición formal de un AP (estado inicial
  fuera de `Q`, símbolo inicial de pila fuera de `Γ`, transiciones con
  estados o símbolos no declarados...), el `-type` indicado no coincide con
  lo que hay realmente en el fichero, argumentos de línea de comandos
  incorrectos o incompletos (`-config` y `-type` son obligatorios), o un
  fichero de `-in`/`-out` que no se puede abrir.
- **Errores no críticos** (se avisa con `Warning:` por `stderr` y se
  continúa): una cadena de entrada con un símbolo fuera de `Σ` (se avisa,
  se salta esa cadena y se sigue con las demás, indicando la línea exacta
  del fichero en la que estaba), una transición duplicada en la
  definición, o una opción inválida del menú interactivo.

Tengo un conjunto de ficheros de prueba en `data/automata/` pensado
específicamente para disparar cada uno de estos errores por separado (uno
por tipo de fallo), útiles tanto para probar el programa a mano como de
referencia para entender qué comprueba cada validación.

## Compilación y ejecución

```bash
make            # compila el simulador -> bin/main
make test       # compila y ejecuta la batería de tests -> bin/run_tests
make clean      # borra build/ y bin/
```

El programa se invoca así:

```bash
./bin/pda -config <fichero> -type <apv|apf> [-trace] [-in <fichero>] [-out <fichero>]
```

| Opción              | Obligatoria | Descripción                                                                      |
|---------------------|:-----------:|-----------------------------------------------------------------------------------|
| `-config <f>`       | Sí          | Fichero con la definición del autómata                                            |
| `-type <apv\|apf>`  | Sí          | Tipo del autómata de ese fichero (`apv` = vaciado de pila, `apf` = estado final)   |
| `-trace`            | No          | Muestra la traza de la simulación                                                 |
| `-in <f>`           | No          | Cadenas a comprobar, una por línea; sin ella se piden por teclado                  |
| `-out <f>`          | No          | Fichero donde escribir la traza; sin ella se escribe en pantalla                   |

Ejemplo, comprobando varias cadenas desde fichero con traza a otro fichero:

```bash
./bin/main -config data/automata/APf-cadenas_an_bn.txt -type apf \
           -in data/strings/cadenas_apf.txt \
           -trace -out /tmp/traza.txt
```

Sin `-in`, el programa entra en un menú interactivo desde el que se puede
elegir comprobar cadenas por teclado (`.` para la cadena vacía, `..` para
salir) o desde un fichero.

## Tests

Uso [GoogleTest](https://github.com/google/googletest), vendorizado en
`libs/googletest/` (sin submódulo de git, para no depender de red al
compilar). `make test` compila y lanza toda la batería. Cada clase tiene su
fichero de test correspondiente en `tests/`, siguiendo la misma
organización por carpetas que `include/`, y cubriendo tanto el
comportamiento esperado como sus principales casos de error.  

Una vez compilados, los tests se pueden ejecutar así:

```bash
./bin/run_tests
```

## Contenedor Docker

El proyecto incluye `Dockerfile`, `docker-compose.yml` y una configuración
de devcontainer para trabajar sin depender del entorno local.

```bash
docker compose build       # construye la imagen
docker compose run --rm dev bash    # abre una shell dentro del contenedor
make test                  # dentro ya del contenedor, como en local
```

También se puede abrir la carpeta del proyecto en VS Code y elegir *Dev
Containers: Reopen in Container*, que deja el entorno listo (compilador,
`make`, y el resto de dependencias) sin ningún paso manual.

## Repositorio

Código fuente completo en GitHub: `https://github.com/Marco-Perez-Padilla/Complejidad-Computacional.-P01-PAD`