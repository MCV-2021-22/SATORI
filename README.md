# SATORI
SATORI game

## Composicion basica del sistema GAS
1. Ability System Component : contiene Habilidades y Atributos, que pueden entenderse como el core del sistema de habilidades.
1. Ability : Puede entenderse como una determinada habilidad, como bola de fuego, sprint etc.
1. Attribute Y AttibuteSet : El atributo representa un determinado atributo del rol, como salud, man¨¢, etc.
1. Tags :Representa alg¨²n estado o propiedad. Por ejemplo, el personaje est¨¢ en estado de Burning, entonces la etiqueta de este estado es Character.State.Burning.
1. Gameplay Effect(GE) : Conjuntos de datos que representa la modificacion de ciertos atributos y etiquetas. Por ejemplo, GE_Damage debe configurarse para modificar Add -20 to Attribute: Health.
Tambi¨¦n puede agregar y modificar etiquetas, o verse afectado por etiquetas.
1. Ability Task : Representada como una tarea en Habilidad. or ejemplo, una tarea que casi todas las habilidades usar¨¢n es PlayMontageAndWait, puede ver que crea y devuelve una tarea as¨ªncrona.