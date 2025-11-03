// PIP3 - Personal Identity Profile Version 3
// Created by Michael Robbins and Rick Good
// PC program created 10/20/2018 - 6/26/2022 by Walter D. Pullen (Astara@msn.com)
// See also: https://www.sevenray.org/education/

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

// Definitions

#define szDate "Junio 26, 2022"
#define szAddr "Astara@msn.com"
#define szSave "pip3.sav"
#define fFalse 0
#define fTrue 1
#define cRay 7
#define cPart 4
#define cInit 27
#define cStatement 141
#define cAspire 14
#define cAspire2 13
#define cTrait 28
#define cTrait2 5
#define cResponse 7
#define colMax 80

#define cchSzMax (255*2)
#define chNull '\0'
#define FOdd(n) ((n) & 1)
#define NAbs(b) abs(n)
#define ChHex(n) ((char)((n) < 10 ? '0'+(n) : 'a'+(n)-10))
#define Rgb(bR, bG, bB) \
  (((long)(bR)) | ((long)(bG)<<8) | ((long)(bB)<<16))
#define RgbR(l) ((int)((long)(l) & 255))
#define RgbG(l) ((int)(((long)(l) >> 8) & 255))
#define RgbB(l) ((int)((long)(l) >> 16))
#define ClearB(pb, cb) memset(pb, 0, cb)

enum _entrycode {
  ecNormal = 0,
  ecRetry = 1,
  ecNew = 2,
  ecDone = 3,
  ecExit = 4,
  ecLast = 5,
  ecUndo = 6,
  ecGoto = 7,
  ecPart = 8,
  ecLate = 9,
};


// Types

typedef int flag;
typedef float real;

typedef struct _statement {
  int ray;
  int nType;
  flag fReverse;
  flag fSkip;
  char *sz;
} STATEMENT;

typedef struct _trait {
  int ray;
  char *sz;
} TRAIT;

typedef struct _response {
  int res;
  int resf;
} RESPONSE;

typedef struct _powerray {
  int typea;
  int typeb;
  int gen;
  int tot;
} POWRAY;


// Tables

const char *rgInit[cInit] = {
"nombre completo",
"ubicaci¢n actual (ciudad, estado/provincia, pa¡s)",
"lugar de nacimiento (ciudad, estado/provincia, pa¡s)",
"fecha y hora de nacimiento (mes, d¡a, a¤o, minuto)",
"ocupaci¢n actual",
"ocupaciones anteriores",
"especialidades acad‚micas y materias favoritas",
"aficiones y pasatiempos",
"principales metas de vida",
"factores que lo condujeron a estudiar esoterismo",
"cantidad de tiempo que ha estudiado esoterismo",
"cantidad de tiempo que ha estudiado a Alice Bailey",
"cantidad de tiempo que ha estudiado los siete rayos",
"cantidad de tiempo que ha estudiado astrolog¡a mundial",
"cantidad de tiempo que ha estudiado astrolog¡a esot‚rica",
"rayo mon dico estimado (1-7)",
"confianza en la estimaci¢n del rayo mon dico",
"rayo del Alma estimado (1-7)",
"confianza en la estimaci¢n del rayo del Alma",
"rayo de la personalidad estimado (1-7)",
"confianza en la estimaci¢n del rayo de la personalidad",
"rayo del veh¡culo mental estimado (1-7)",
"confianza en la estimaci¢n del rayo mental",
"rayo del veh¡culo astral estimado (1-7)",
"confianza en la estimaci¢n del rayo astral",
"rayo del veh¡culo f¡sico estimado (1-7)",
"confianza en la estimaci¢n del rayo f¡sico",
};

const char *rgInit2[cInit] = {
"Nombre",
"Ubicaci¢n actual",
"Lugar de nacimiento",
"Fecha y hora de nacimiento",
"Ocupaci¢n actual",
"Ocupaciones anteriores",
"Materias favoritas",
"Aficiones",
"Objetivos de vida",
"Factores que lo condujeron a estudiar esoterismo",
"Por cu nto tiempo ha estudiado esoterismo",
"Por cu nto tiempo ha estudiado a Alice Bailey",
"Por cu nto tiempo ha estudiado los siete rayos",
"Por cu nto tiempo ha estudiado astrolog¡a mundial",
"Por cu nto tiempo ha estudiado astrolog¡a esot‚rica",
"Rayo mon dico estimado",
"Confianza en la estimaci¢n del rayo mon dico",
"Rayo del Alma estimado",
"Confianza en la estimaci¢n del rayo del Alma",
"Rayo de la personalidad estimado",
"Confianza en la estimaci¢n del rayo de la personalidad",
"Rayo del veh¡culo mental estimado",
"Confianza en la estimaci¢n del rayo mental",
"Rayo del veh¡culo astral estimado",
"Confianza en la estimaci¢n del rayo astral",
"Rayo del veh¡culo f¡sico estimado",
"Confianza en la estimaci¢n del rayo f¡sico",
};

const char *rgInitDef[cInit] = {
"Juan P‚rez",
"Cierto lugar, Pa¡s",
"Lugar de nacimiento, Pa¡s",
"1 de enero de 2000, a las 12:34 PM",
"Elaboraci¢n de artilugios",
"Elaboraci¢n de antiguos artilugios",
"Matem ticas",
"Computadoras",
"Crecimiento espiritual",
"Buscar un crecimiento espiritual m s profundo",
"Desde 2014",
"Desde 2015 cuando le¡ Iniciaci¢n Humana y Solar (IHS)",
"Desde 2016 cuando le¡ los Rayos y las Iniciaciones (RI)",
"Desde 2013",
"Desde 2017 cuando le¡ Astrolog¡a Esot‚rica (AE)",
"1",
"No muy seguro",
"2",
"Algo seguro",
"4",
"Una cantidad equilibrada",
"5",
"Es la elecci¢n l¢gica",
"6",
"Siento que est  bien",
"7",
"Muy seguro",
};

#define G 1
#define a 2
#define b 4
#define Ga (G|a)
#define Gb (G|b)
#define norm 0
#define REVERSE 1
#define inc 0
#define SKIP 1

const STATEMENT rgStatement[cStatement] = {
{3, a, norm, inc, "Soy un pensador abstracto h bil, que naturalmente encuentra los patrones abstractos de las relaciones filos¢ficas, matem ticas, te¢ricas o sist‚micas mucho m s intrigantes que el descubrimiento y la aplicaci¢n de conocimientos pr cticos, concretos, art¡sticos o incluso cient¡ficos, incluso si ese conocimiento espec¡fico es intr¡nsecamente fascinante o inmediatamente £til."},
{6, Gb, norm, inc, "Si realmente amo a alguien, ir‚ \"hasta el conf¡n del mundo\" para mostrar c¢mo me siento."},
{6, b, norm, inc, "Soy conocido por mi optimismo, por mi entusiasmo, por mi actitud de \"no podr n conmigo\", \"nunca me rendir‚\"."},
{4, b, norm, inc, "He vivido mi vida tanto en las cumbres como en los barrancos de la experiencia humana."},
{6, a, norm, inc, "Siento profundamente que estoy siendo guiado. Tengo completa fe en mi gu¡a y la sigo."},
{4, a, norm, inc, "Crear belleza en todo lo que hago es absolutamente indispensable para m¡. Busco convertirme en el \"artista de mi vida\"."},
{7, G, norm, inc, "Soy el indicado para \"enderezar\" y \"ordenar las cosas \". \"Un lugar para cada cosa y cada cosa en su lugar\" es mi lema."},
{1, a, norm, inc, "Sea lo que sea lo que la vida me depare, me mantengo fuerte, firme, constante, sin miedo, persistiendo hasta el final."},
{1, b, norm, inc, "Destruyo lo viejo e innecesario con mucha menos vacilaci¢n que la mayor¡a de las personas."},
{2, G, norm, inc, "Casi siempre me <i>identifico</i> (inmediata y profundamente) con los sentimientos y estados mentales de las personas, absorbi‚ndolos como si fueran m¡os."},
{1, a, norm, inc, "Soy un l¡der natural."},
{3, a, norm, inc, "Sobre todo soy un <i>pensador</i>, y uso mi intelecto para pensar en los <i>t‚rminos m s amplios</i> sobre cualquier tema que est‚ considerando."},
{1, a, norm, inc, "Comparado con otros, tengo una voluntad extremadamente fuerte y firme."},
{7, b, norm, inc, "Soy una \"persona de organizaciones\", en realidad, una \"persona de re-organizaciones\", cuya fuerza predominante radica en dise¤ar, construir o administrar de manera eficiente todos los muchos detalles del nuevo tipo de organizaciones (grandes o peque¤as) necesarias para ayudar a la humanidad a resolver sus muchos problemas, ya sean legales, financieros, sociales, religiosos o gubernamentales, etc."},
{5, G, norm, inc, "Soy particularmente bueno en matem ticas aplicadas, el tipo de matem ticas que requiere c lculos precisos y habilidad en varias operaciones aritm‚ticas que se pueden aplicar a la soluci¢n de problemas pr cticos."},
{1, G, norm, inc, "Cuando me comparo con los dem s, puedo decir honestamente que soy una persona extraordinariamente poderosa: la fuerza y la firmeza son dos de mis virtudes <i>dominantes</i>."},
{4, b, norm, inc, "Peleo, luego me arreglo, peleo de nuevo, luego me arreglo de nuevo. ­Parece que no puedo evitar hacer ambas cosas!"},
{4, b, norm, inc, "Me encanta ser dram tico y mi vida est  llena de experiencias dram ticas y contrastes emocionales."},
{2, b, norm, inc, "Dame <i>sabidur¡a</i> por sobre todo. La valoro m s que muchas cualidades esenciales: poder, amor, inteligencia, belleza, conocimiento, idealismo, orden, etc. La sabidur¡a es lo primero."},
{1, G, norm, inc, "Soy quien dirige a los dem s."},
{4, b, norm, inc, "M s que la de la mayor¡a de la gente, mi vida ha estado llena de conflictos (internos, externos o ambos); muy a menudo me he sentido dividido entre fuerzas en conflicto."},
{4, b, norm, inc, "Mi enfoque habitual para hacer las cosas se caracteriza por estallidos de actividad incesante, seguidos de per¡odos casi iguales de letargo total durante los cuales (aunque siento que deber¡a estar activo) simplemente no puedo hacer nada. Entonces, de repente, mi estado de  nimo cambia y de nuevo entro en acci¢n."},
{5, G, norm, inc, "Con frecuencia abordo problemas o tareas a trav‚s de la cuantificaci¢n. Analizo o eval£o un problema o tarea en t‚rminos de n£meros y, en consecuencia, determino una soluci¢n."},
{2, b, norm, inc, "Todos los caminos hacia el centro del ser, ya sean los caminos del poder, el amor, la inteligencia, la belleza, el conocimiento, el ‚xtasis, la magia, etc., tienen el mismo valor, pero mi camino particular es la b£squeda eternamente paciente de la sabidur¡a pura del coraz¢n, de la iluminaci¢n completa y de la verdad absoluta."},
{4, a, norm, inc, "Simplemente amo el color y tengo un sentido del color altamente desarrollado que expreso, de una forma u otra, en cada oportunidad."},
{6, a, norm, inc, "Cuando considero todas las diversas formas de elevar la humanidad, creo que la fe y la creencia en Dios, en un poder superior o en alguna gran fuente de verdad son los m s importantes."},
{1, b, norm, inc, "Estoy preparado, equipado e inclinado a forzar los asuntos y dar ultim tums siempre que lo crea necesario."},
{5, a, norm, inc, "Le doy un valor muy alto a la exactitud estricta y f ctica, incluso cuando otros pueden perder la paciencia con tal precisi¢n y exactitud."},
{7, a, norm, inc, "Casi siempre soy extremadamente cuidadoso al manejar incluso los detalles m s peque¤os. Quiero que las cosas se hagan correctamente."},
{2, G, norm, inc, "Siempre busco desarrollar una comprensi¢n completa de las personas, una comprensi¢n profunda, intuitiva y 'de coraz¢n a coraz¢n'."},
{5, a, norm, inc, "Siempre que realizo un estudio, lo hago de manera rigurosa y sistem tica, dominando a fondo todos los detalles f cticos de mi tema."},
{1, Gb, norm, inc, "Estoy bien dotado con el poder din mico para romper obst culos, y ­lo hago!"},
{5, b, norm, inc, "Estoy involucrado activamente (vocacionalmente o por pasatiempo) en una o m s de las \"ciencias exactas \" tales como f¡sica, qu¡mica, astronom¡a, biolog¡a, electr¢nica, ciencias de la computaci¢n, etc. o en alguna de sus muchas combinaciones o aplicaciones."},
{5, b, norm, inc, "Me acerco a la resoluci¢n de problemas a trav‚s de la observaci¢n de primera mano, examinando cuidadosamente todos los detalles. Luego empiezo a experimentar met¢dicamente, usando un procedimiento de \"prueba y error\", y persisto hasta que me he probado a m¡ mismo que tengo una soluci¢n que realmente funciona."},
{1, b, norm, inc, "Soy uno de los que \"toma el Reino de los Cielos por la fuerza\"."},
{2, G, norm, inc, "Tengo una capacidad muy desarrollada para percibir con claridad y con previsi¢n intuitiva. Esta habilidad es especialmente aguda cuando se aplica a la comprensi¢n de las personas: sus potenciales, motivos, problemas y relaciones."},
{2, a, norm, inc, "Tengo una necesidad profundamente arraigada de nutrir y consolar a las personas, incluso si no las conozco personalmente."},
{7, a, norm, inc, "Soy un planificador de tiempo habitual y un creador de listas; as¡ es como me aseguro de hacer todo lo que tengo que hacer, en el momento en que debe hacerse y en el tiempo que tengo para hacerlo."},
{3, a, norm, inc, "Aunque hay muchas actividades que merecen la pena, vivo mi vida principalmente como intelectual, principalmente involucrado en el mundo del pensamiento."},
{1, b, norm, inc, "No tengo ninguna dificultad real para dar por terminadas o descartar las cosas. De hecho, a menudo me emociona, especialmente cuando algo viejo y gastado tiene que ser eliminado para que algo nuevo y fresco pueda ocupar su lugar."},
{5, b, norm, inc, "Disfruto retocando y desmontando cosas (piezas de equipo, dispositivos mec nicos, objetos varios, etc.) para ver c¢mo se ensamblan, c¢mo funcionan y c¢mo se pueden reparar."},
{6, G, norm, inc, "A menudo le prometo a Dios (o a m¡ mismo o a los dem s) que defender‚ alg£n ideal elevado."},
{6, a, norm, inc, "Anhelo vivir mi vida con fidelidad y lealtad, sirviendo fervientemente a quienes amo."},
{3, G, norm, inc, "Hablo mucho, m s que la mayor¡a de la gente. Por lo general, mi mente est  llena de muchos pensamientos interesantes y solo tengo que darles expresi¢n verbal."},
{1, G, norm, inc, "Tengo una habilidad sorprendente para iniciar la acci¢n, impulsando a otros a la actividad. Doy el martillazo que hace vibrar las cosas."},
{3, a, norm, inc, "Mi estilo de escritura suele ser abstracto, complejo y bastante complicado. Disfruto de la elaboraci¢n y la ramificaci¢n, y extiendo mi mente a numerosos campos para combinar muchas corrientes de pensamiento."},
{4, a, norm, inc, "Experimentar la belleza que surge de la disposici¢n armoniosa y est‚ticamente atractiva de mi entorno personal y social es vital para mi felicidad."},
{7, b, norm, inc, "Mi tarea es \"traer el Cielo a la Tierra\", no solo en general, como lo hacen todos los servidores, sino a trav‚s de la <i>excelente expresi¢n de formas celestiales arquet¡picas</i> a trav‚s de <i>formas terrenales perfeccionadas</i>."},
{3, G, norm, inc, "Disfruto \"calculando las probabilidades\", especulando y conjeturando sobre las probabilidades."},
{7, a, norm, inc, "D‚jame los detalles pr cticos. Los organizar‚ y ejecutar‚ de la manera m s eficiente."},
{3, G, norm, inc, "Razono con una facilidad inusual sobre cuestiones te¢ricas sumamente abstractas en las que debe entenderse clara y exactamente una red compleja de relaciones abstractas. (Por ejemplo: matem ticas superiores, filosof¡a acad‚mica, teor¡a econ¢mica, proyecciones de materias primas y valores, tendencias hist¢ricas, etc.)"},
{7, a, norm, inc, "En casi todo lo que hago, lo m s natural para m¡ es desarrollar una rutina o ritual personal, un patr¢n regular de acci¢n que me funciona de manera eficiente y que me gusta repetir."},
{4, G, norm, inc, "Concedo demasiado f cilmente cuando, en cambio, deber¡a defender los principios."},
{1, a, norm, inc, "Soy una persona decidida, muy valiente ante todo tipo de peligros."},
{3, b, norm, inc, "Cuando pienso en todas las diversas formas de elevar la humanidad, creo que los buenos negocios, las econom¡as inteligentes, las pr cticas monetarias s¢lidas y las oportunidades financieras generalizadas son las m s importantes."},
{5, G, norm, inc, "Me gusta leer publicaciones acad‚micas o t‚cnicas que explican, con considerable detalle, los desarrollos en ciertos campos especializados de investigaci¢n."},
{1, G, norm, inc, "Cuando alguien o algo se me enfrenta, mi <i>voluntad de conquista</i> aumenta de inmediato."},
{5, G, norm, inc, "Me anima el esp¡ritu del descubrimiento cient¡fico y disfruto y me satisface la b£squeda de la investigaci¢n cient¡fica."},
{1, a, norm, inc, "Tengo un firme prop¢sito en casi todo lo que pienso o hago."},
{4, G, norm, inc, "No solo me atraen irresistiblemente las artes creativas, sino que tambi‚n expreso y libero mis emociones a trav‚s de alguna forma de creatividad art¡stica."},
{5, G, norm, inc, "Por lo general, tengo una actitud muy objetiva, que surge de un examen sereno, desapasionado, sumamente mental y anal¡tico de las situaciones de la vida."},
{1, a, norm, inc, "Tengo la fuerza y la independencia para estar solo. Necesito el apoyo y la compa¤¡a de los dem s mucho menos que la mayor¡a de la gente."},
{2, G, norm, inc, "Debido a que a mi coraz¢n le hace bien ver a las personas crecer y desarrollarse, uno de mis mayores placeres es ser maestro."},
{6, a, norm, inc, "Tengo a mis seres queridos muy cerca de m¡. Me siento absolutamente dedicado a ellos e inseparablemente conectado."},
{7, b, norm, inc, "Soy un ritualista pr ctico que usa el movimiento organizado, el pensamiento regulado y la palabra r¡tmica (as¡ como el lenguaje m gico del color, la m£sica y el s¡mbolo) para invocar las fuerzas superiores en ayuda de la humanidad."},
{2, a, norm, inc, "A mi manera tranquila, gentil y amorosa, me aseguro de enfatizar lo positivo. Casi siempre tengo algo bueno que pensar o decir sobre la gente."},
{2, a, norm, inc, "Atraigo a la gente a trav‚s de la calidez y el magnetismo, y mi amor ayuda a mantenerlos unidos."},
{1, G, norm, inc, "Por alguna raz¢n, me inclino fuertemente a ser dominante. No rehuyo imponer autoridad."},
{5, Ga, norm, inc, "Mi intelecto es preciso, muy anal¡tico y muy centrado en el conocimiento f ctico y en cuestiones concretas y espec¡ficas."},
{5, a, norm, inc, "Brindo un estudio minucioso y detallado (aunque intuitivo) incluso de los temas m s amplios; algunos llaman a mi enfoque pretencioso, pero mi objetivo es lograr una comprensi¢n completa de todos los factores relacionados, familiariz ndome tan completamente con un tema que (usando mi m‚todo preferido de \"meticulosidad total\") absorbo ese tema por completo."},
{6, b, norm, inc, "Cuando persigo un ideal elevado, me dedico a ‚l de manera concentrada y con ferviente intensidad emocional."},
{6, G, norm, inc, "Me aferro a ciertos ideales elevados que con frecuencia y con entusiasmo reafirmo a m¡ mismo y enfatizo a los dem s. Estos ideales son mi \"estrella gu¡a\", y por ellos trazo mi curso en la vida."},
{7, G, norm, inc, "Creo que las apariencias son muy importantes. Invierto una energ¡a considerable en promover una buena apariencia en todos los aspectos de mi vida y circunstancias."},
{6, b, norm, inc, "Siempre que \"voy por\" algo, lo hago con pasi¢n, con un entusiasmo ilimitado."},
{3, b, norm, inc, "Mi mente  gil e inteligente est  bien preparada para trabajar en el campo de la econom¡a y las finanzas."},
{5, b, norm, inc, "Cuando surgen problemas t‚cnicos, estoy en mi elemento actuando como el \"solucionador de problemas\"."},
{6, a, norm, inc, "Oro con total confianza en que mis oraciones siempre sean contestadas por Dios o alg£n Poder Superior, y con humilde voluntad de aceptar cualquier respuesta o soluci¢n que reciba."},
{4, a, norm, inc, "Me encanta crear conjuntos, composiciones o mezclas coloridas y est‚ticamente atractivas al producir imaginativamente una unidad art¡stica de cosas que generalmente no se encuentran unidas."},
{4, b, norm, inc, "La sensaci¢n de estar \"deprimido\" y desanimado me invade con frecuencia, pero el estado de  nimo pronto mejora y vuelvo a estar  \"animado\" y listo para intentarlo de nuevo."},
{3, b, norm, inc, "Disfruto moviendo dinero aqu¡ y all  de acuerdo con mi mejor anticipaci¢n de ganancias."},
{3, b, norm, inc, "Mi naturaleza inteligentemente adaptable y mi astuta previsi¢n de las tendencias comerciales hacen de la actividad empresarial mi campo natural de expresi¢n."},
{3, b, norm, inc, "M s que a muchas personas, me gusta \"ser inteligente\" al especular con el dinero, invertir y usar mi ingenio para generar ganancias."},
{4, G, norm, inc, "Pienso, escribo y hablo con palabras e im genes coloridas."},
{1, b, norm, inc, "Cuando me encuentro en una situaci¢n personal o profesional que no deber¡a continuar como est , prefiero ajustes dr sticos y liberadores que acaben abruptamente con las viejas condiciones."},
{3, b, norm, inc, "Disfruto especialmente aquellos aspectos de cualquier empresa comercial que me exigen organizar transacciones de manera inteligente y creativa."},
{5, a, norm, inc, "Cuando explico algo, me aseguro de ser extremadamente l£cido, l¢gico y espec¡fico (ofreciendo ejemplos concretos para ilustrar mi punto), para que mi oyente pueda captar cada detalle con total claridad."},
{2, G, norm, inc, "Medito y estudio continuamente con el prop¢sito de lograr la iluminaci¢n completa y una comprensi¢n profunda de la verdad, aunque tal comprensi¢n exhaustiva que requiere mucho tiempo no tenga una aplicaci¢n pr ctica inmediata en las circunstancias actuales."},
{4, G, norm, inc, "Me encanta el color; pero no solo me encanta, sino que <i>lo produzco</i> en todo lo que creo, y <i>expreso mi sentido del color</i> en la ropa que uso."},
{7, a, norm, inc, "Soy de los que exige que todas las cosas se hagan con decencia y en orden."},
{5, b, norm, inc, "Cuando miro una pieza de equipo, mi primer pensamiento es \"¨c¢mo funciona?\". Luego me pongo a averiguarlo."},
{3, G, norm, inc, "Dedico una gran cantidad de tiempo a comunicar una amplia gama de ideas e informaci¢n a trav‚s de cualquier medio disponible para m¡, ya sea de boca en boca o de muchos tipos de medios."},
{6, G, norm, inc, "Me encuentro marchando hacia la Luz."},
{6, G, norm, inc, "Con frecuencia experimento fuertes sentimientos de devoci¢n."},
{2, b, norm, inc, "Amo y respeto profundamente a Cristo y Su ejemplo de amor puro, pero mi naturaleza me inclina a buscar la iluminaci¢n y la realizaci¢n del Ser a trav‚s de la aplicaci¢n de mi coraz¢n y mente meditativa a la sabidur¡a profunda ense¤ada por el Buda."},
{7, G, norm, inc, "Concentro mucha atenci¢n en las reglas y reg¡menes corporales para mantener mi cuepro vitalmente saludable, o en programas para cultivar todo su potencial."},
{3, G, norm, inc, "Soy muy bueno \"tramando y planificando\", pensando en lo que har‚ con muchos movimientos por delante. Realmente soy un excelente estratega."},
{1, G, norm, SKIP, "En comparaci¢n con los dem s, soy un individuo poderoso: la fuerza y la firmeza son dos de mis virtudes predominantes."},
{4, Ga, norm, SKIP, "No solo me atraen irresistiblemente las artes creativas, sino que tambi‚n expreso y libero mis emociones a trav‚s de alguna forma de creatividad art¡stica."},
{5, b, norm, inc, "Me siento como en casa en los laboratorios y disfrutar¡a trabajando en alg£n tipo de entorno de laboratorio, donde el conocimiento se persigue y se confirma experimentalmente."},
{6, G, norm, inc, "Deseo entregarme totalmente a una causa o ideal dignos."},
{7, a, norm, inc, "Soy alguien que realmente entiende el valor y la importancia de respetar la ley y \"jugar seg£n las reglas\"."},
{2, b, norm, inc, "A trav‚s de un estudio profundo y una meditaci¢n profunda abarcante, estoy decidido a lograr una comprensi¢n intuitiva, totalmente inclusiva pero meticulosamente detallada de la belleza modelada del todo."},
{6, a, norm, inc, "Amo adorar a Dios y estoy lleno de reverencia por los grandes santos y sabios que ejemplifican los caminos del Creador."},
{3, b, norm, inc, "Soy t ctico y estratega, una persona a la que siempre le gusta tener un \"plan de juego\"."},
{6, b, norm, inc, "Soy un 'cruzado' moderno en una causa noble."},
{2, a, norm, inc, "Mi camino es compartir libremente la radiante luz del amor sincero con tantas personas como sea posible."},
{1, Gb, norm, inc, "Digo la verdad de manera clara y directa; puede herir o molestar a algunos, pero la verdad es la verdad y no tengo miedo de contarla tal como es."},
{3, G, norm, inc, "La inusual flexibilidad y fertilidad de mi mente hacen que sea f cil para m¡ modificar y adaptar las ideas de todo tipo. Soy muy bueno ayudando a las personas a ver las cosas desde una variedad de  ngulos novedosos, multiplicando as¡ sus opciones."},
{7, G, REVERSE, inc, "Algunas personas dicen que tengo la \"cabeza en las nubes\", pero en realidad ni me preocupo yo ni preocupo a los dem s por lo que considero peque¤as cosas sin importancia. Aunque para otros esas cosas son a menudo importantes, para m¡ son, por lo general, nimiedades con las que no me puedo molestar."},
{4, a, norm, inc, "Siempre me encuentro \"en el medio\", un puente mediador y un agente de reconciliaci¢n entre opiniones contradictorias y fuerzas en conflicto."},
{5, a, norm, inc, "No solo disfruto investigando, sino que tambi‚n tengo la paciencia y la perseverancia para rastrear el hecho m s peque¤o hasta su origen a fin de garantizar la precisi¢n de mi investigaci¢n."},
{4, a, norm, inc, "Soy un \"pacificador\" h bil porque casi siempre tengo un verdadero entendimiento de \"ambos lados\" de las situaciones conflictivas y realmente disfruto del proceso a menudo estresante de hacer concesiones y compromisos para as¡ encontrar los puntos de mutuo acuerdo que son necesarios para resolver el conflicto en armon¡a."},
{2, a, norm, inc, "Es m s f cil para m¡ que para la mayor¡a establecer relaciones amables, c lidas y amorosas con las personas que conozco."},
{2, G, norm, inc, "Es muy natural para m¡ abordar cualquier objetivo importante con paciencia y sensibilidad. Logro mucho con tacto, consideraci¢n y el uso sabio de la acci¢n lenta."},
{7, G, norm, inc, "Le doy una gran importancia a la manera y el estilo en que se hace una cosa. Soy excelente para hacer todo \"en buena forma\"."},
{2, b, norm, inc, "Aunque hay muchas formas de expresar el amor, mi forma particular es combinar el amor con la sabidur¡a austera en lugar de simplemente derramar amor puro e incondicional."},
{5, a, norm, inc, "Con la intenci¢n de distinguir una cosa de otra, busco descubrir o aprender la clasificaci¢n o el nombre correctos de todo lo que examino, de modo que pueda saber exactamente qu‚ es y qu‚ no es una cosa."},
{3, G, norm, inc, "Soy un individuo <i>muy</i> activo; muchos han llamado, \"hiperactivo\". Hay tantas cosas que hacer y uno simplemente no puede hacerlas movi‚ndose lentamente."},
{7, a, norm, inc, "­Cuando arreglo las cosas, soy m s exigente que la mayor¡a de la gente acerca de hacer el arreglo exacto, adecuado o \"justo\"!"},
{7, b, norm, inc, "Estoy constantemente reestructurando el orden existente de las cosas, renovando y redise¤ando los aspectos pr cticos de la vida diaria, ya sea en mi propia esfera personal y profesional, o en un contexto mucho m s amplio."},
{1, a, norm, inc, "Normalmente soy el poder en el centro de las cosas; es una posici¢n que es completamente natural para m¡."},
{6, b, norm, inc, "Cuando realmente aspiro a alcanzar una meta, me \"enciendo\" y \"ardo de entusiasmo\" hasta que llego all¡."},
{3, a, norm, inc, "Opero sobre la premisa filos¢fica de que la verdad se puede alcanzar de hecho sin experimentaci¢n cient¡fica, principalmente mediante el uso de un pensamiento deductivo, l¢gico y riguroso, siempre que las premisas sean correctas y el razonamiento s¢lido."},
{4, a, norm, inc, "Debido a que la falta de armon¡a siempre me hace sentir extremadamente inc¢modo, act£o de inmediato para restaurar la armon¡a de la manera m s r pida y h bil posible."},
{3, a, norm, inc, "Soy un fil¢sofo, un intelectual. En mi b£squeda de una amplia comprensi¢n filos¢fica, el intrincado e infinitamente complejo mundo del intelecto es mi verdadero hogar."},
{1, G, norm, inc, "Tengo una manera magistral de dirigir a las personas para que hagan mi voluntad o la Voluntad de un Ser Superior; de hecho, insisto en que lo hagan."},
{2, a, norm, inc, "A menudo siento una gran simpat¡a y compasi¢n por todas las personas. En momentos tan especiales, amo y perdono a todos incondicionalmente, sin importar qui‚nes sean o lo que hayan hecho."},
{7, b, norm, inc, "Tengo una habilidad muy real para traer un orden r¡tmico al caos de la vida humana, \"aterrizando\" las ideas transformadoras en actividades pr cticas, y \"conduci‚ndolas\" hasta su expresi¢n perfecta."},
{7, b, norm, inc, "Soy un 'mago' moderno. Al estar suficientemente familiarizado con las fuerzas ang‚licas y elementales de la naturaleza, utilizo cuidadosa y h bilmente las leyes pr cticas de la voluntad y el pensamiento para vincular el esp¡ritu con la materia en beneficio de la humanidad."},
{1, G, norm, inc, "Cuando decido qu‚ se debe hacer, me niego absolutamente a permitir que los sentimientos, las emociones o los apegos a las personas me impidan ejecutar plenamente mi intenci¢n."},
{4, b, norm, inc, "Soy tanto valiente como cobarde (a menudo al mismo tiempo) y la expresi¢n de uno u el otro depende de mi estado de  nimo, que a menudo cambia."},
{4, G, norm, inc, "Cada que la tensi¢n se acumula a menudo digo o hago algo gracioso para liberarla."},
{7, a, norm, inc, "Siento una gran satisfacci¢n al dise¤ar o participar en programas de acci¢n altamente estructurados y minuciosamente planificados, en los que los procedimientos apropiados est n claramente detallados y los resultados previstos est n claramente definidos."},
{3, a, norm, inc, "Me encanta la gimnasia mental y la demostraci¢n de virtuosismo intelectual."},
{4, G, norm, inc, "Mi forma habitual de escribir y hablar es expresar mis pensamientos con sensibilidad, libre, espont nea y, a menudo, po‚ticamente."},
{4, G, norm, inc, "Soy muy bueno para entretener y divertir a la gente con conversaciones brillantes e imaginativas, pero tambi‚n es probable que dando un giro sobre m¡ mismo me vuelva introspectivo o incluso sombr¡amente silencioso."},
{5, b, norm, inc, "No solo tengo un gran inter‚s en inventar, sino tambi‚n cierta habilidad como inventor en el campo de la tecnolog¡a."},
{2, a, norm, inc, "Practico constantemente la bondad amorosa en todos los encuentros y experiencias de mi vida."},
{6, G, norm, inc, "Siempre que mis preciados ideales son desafiados, surjo celosamente en su defensa."},
{6, a, norm, inc, "A menudo anhelo estar donde todo es puro y perfecto, ll emele \"utop¡a\" o \"para¡so\"."},
{7, b, norm, inc, "La forma en que vivo mi vida se puede describir con precisi¢n como \"ritualista\". La ceremonia, la magia y la manifestaci¢n resultante son temas importantes en mi conciencia y actividad."},
};

const TRAIT rgAspire[cAspire] = {
{1, "Aspiro a ser un din mico, poderoso y benevolente l¡der: fuerte, firme y r pido en mi servicio a la Ley Mayor y en mi defensa al Bien M s grande."},
{2, "Aspiro a una profunda e intuitiva comprensi¢n amorosa de la gente, para que as¡ pueda nutrirla, y ayudarla a desarrollar su potencial m s elevado."},
{3, "Aspiro a una mentalidad plena de recursos y tan vers til que pueda aplicarla de forma creativa a  numerosas empresas intelectualmente desafiantes."},
{4, "Aspiro a expresar gran maestr¡a en todos los aspectos de mi vida, para as¡ inspirar a otros una mayor creatividad y un amor m s grande  a la belleza."},
{5, "Aspiro a involucrarme vitalmente en el descubrimiento de las claves y soluciones a los misterios de la Naturaleza a trav‚s de la investigaci¢n cient¡fica y la experimentaci¢n."},
{6, "Aspiro a vivir con fe ardiente y compromiso apasionado, inquebrantablemente leal a mi sistema de valores y fiel a mis m s elevados ideales, a mi \"Visi¢n de lo Justo\"."},
{7, "Aspiro a llevar a un orden r¡tmico bello el caos de la vida humana, a trav‚s de la \"concreci¢n\" de ideas transformadoras en actividades pr cticas y \"verlas manifestarse\" en perfecta expresi¢n."},
{1, "Aspiro a romper las cadenas, liberar a pueblos de todas las formas de esclavitud y opresi¢n."},
{2, "Aspiro a una comprensi¢n sabia e intuitiva, profunda y completa de la vida para que as¡ pueda ense¤ar e iluminar a otros."},
{3, "Aspiro a crear una teor¡a filos¢fica de la verdad profundamente comprensiva, totalmente racional, mediante el poder del pensamiento profundo y el razonamiento m s cuidadoso."},
{4, "Aspiro a ser un mediador, un armonizador, \"un puente de reparaci¢n y unidad\", un reconciliador de los conflictos humanos, y un instrumento de paz."},
{5, "Aspiro a inventar o desarrollar esos mecanismos, instrumentos, o t‚cnicas mejoradas, que transformar n e incrementar n la calidad de la vida humana."},
{6, "Aspiro a servir con fe total y devoci¢n absoluta a \"Dios\", a la causa m s digna, o al \"ideal m s elevado\" que puedo concebir."},
{7, "Aspiro a involucrarme profundamente en el arte y la ciencia de la transformaci¢n organizacional creativa, el redise¤o y reestructura de los patrones de la interacci¢n humana, para que as¡ un \"mundo mejor\" pueda emerger m s r pidamente."},
};

const TRAIT rgTrait[cTrait] = {
{7, "Exceso de preocupaci¢n acerca de las reglas y las regulaciones."},
{6, "Entusiasmo descontrolado y equivocado"},
{5, "Demasiado excepticismo."},
{4, "Demasiadas \"subidas\" y \"bajadas\"."},
{7, "Autoexpresi¢n r¡gida e inhibida."},
{6, "Intensidad emocional excesiva."},
{7, "Sumisi¢n a la costumbre y a la rutina."},
{5, "\"Los  rboles no dejan ver el bosque.\""},
{3, "Demasiado di logo y pensamiento hiperactivo que no llega a objetivos tangibles."},
{2, "Amar ser amado."},
{1, "Excesivo ego¡smo, autocentramiento y autoimportancia: determinaci¢n excesiva a ser \"el uno en el centro\"."},
{1, "Ambici¢n ego¡sta de poder."},
{2, "Demasiado estudio sin tomar acci¢n."},
{3, "Orgullo intelectual."},
{4, "Tendencia a la autocontradicci¢n, la inconsistencia y la impredecibilidad."},
{5, "Demasiado an lisis intelectual y minuciosa examinaci¢n cr¡tica."},
{6, "Exceso de predilecci¢n por los dem s."},
{7, "Exceso de preocupaci¢n por el orden y el detalle."},
{4, "Exceso de af n por lograr compromisos, \"la paz a cualquier precio\"."},
{6, "Exceso de af n por la verdad y las creencias, devoci¢n fuera de lugar."},
{3, "Desgaste de energ¡a por exceso de actividad, vida dispersa e intranquilidad."},
{3, "Manipulaci¢n, astucia e intriga."},
{2, "Miedo debido al exceso de sensibilidad."},
{1, "Deseo de dominio y control sobre los dem s."},
{1, "Voluntariedad."},
{2, "Dar(se) a los dem s con demasiada facilidad."},
{5, "Intolerancia a las ideas que no guardan relaci¢n con el conocimiento ya adquirido."},
{4, "Luchas, problemas y confusi¢n debilitantes."},
};

const char *rgszResponse[cResponse+1] = {"Sin Respuesta", "Definitivamente Verdadero", "Generalmente Verdadero", "M s Verdadero que Falso", "En el Medio - Tan Verdadero como Falso", "M s Falso que Verdadero", "Generalmente Falso", "Definitivamente Falso"};
const char *rgszResponseAbbrev[cResponse+1] = {"SR", "DV", "GV", "MV", "EM", "MF", "GF", "DF"};

const char *rgszColor[cRay+3] = {"Black", "Red", "#191970", "#008000", "#c0c000", "#ffa500", "#ff3edf", "#9900CC", "White", "Yellow"};

const int rgPart[cPart] = {cInit, cStatement, cAspire2, cTrait2};

// Characters in 128-255 byte range of codepage IBM-437
const unsigned short rgwch437[128] = {
  0xc7,   0xfc,   0xe9,   0xe2,   0xe4,   0xe0,   0xe5,   0xe7,
  0xea,   0xeb,   0xe8,   0xef,   0xee,   0xec,   0xc4,   0xc5,
  0xc9,   0xe6,   0xc6,   0xf4,   0xf6,   0xf2,   0xfb,   0xf9,
  0xff,   0xd6,   0xdc,   0xa2,   0xa3,   0xa5,   0x20a7, 0x0192,
  0xe1,   0xed,   0xf3,   0xfa,   0xf1,   0xd1,   0xaa,   0xba,
  0xbf,   0x2310, 0xac,   0xbd,   0xbc,   0xa1,   0xab,   0xbb,
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
  0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
  0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
  0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
  0x03b1, 0xdf,   0x0393, 0x03c0, 0x03a3, 0x03c3, 0xb5,   0x03c4,
  0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
  0x2261, 0xb1,   0x2265, 0x2264, 0x2320, 0x2321, 0xf7,   0x2248,
  0xb0,   0x2219, 0xb7,   0x221a, 0x207f, 0xb2,   0x25a0, 0xa0};


// Variables

char rgResInit[cInit][cchSzMax];
RESPONSE rgResS[cStatement];
RESPONSE rgResA[cAspire2];
RESPONSE rgResT[cTrait2];
flag fInfo = fFalse, fHTML = fFalse, fTextFile = fFalse;
FILE *file = NULL;
int cmdarg = 0;


// Functions

long BlendColor(long kv1, long kv2, int n1, int n2)
{
  if (n2 < 1)
    n2 = 1;
  return Rgb((RgbR(kv2) - RgbR(kv1)) * n1 / n2 + RgbR(kv1),
    (RgbG(kv2) - RgbG(kv1)) * n1 / n2 + RgbG(kv1),
    (RgbB(kv2) - RgbB(kv1)) * n1 / n2 + RgbB(kv1));
}

int CchSz(const char *sz)
{
  const char *pch = sz;

  while (*pch)
    pch++;
  return (int)(pch - sz);
}

flag FEqSz(char *sz1, char *sz2)
{
  while (*sz1 && (*sz1 == *sz2))
    sz1++, sz2++;
  return *sz1 == *sz2;
}

flag FPrefixSz(char *sz1, char *sz2)
{
  while (*sz1 && (*sz1 == *sz2))
    sz1++, sz2++;
  return *sz2 == chNull;
}

void ReadLine(FILE *fileIn, char *sz)
{
  int i;
  char ch;

  for (i = 0; i < cchSzMax*2 && !feof(fileIn) && (unsigned char)(ch = getc(fileIn)) >= ' '; i++)
    sz[i] = ch;
  sz[i] = chNull;
  if (ch != '\n')
    ch = getc(fileIn);
}

void PrintBr()
{
  if (fHTML)
    fprintf(file, "<br>");
  fprintf(file, "\n");
}

void Print(char *sz)
{
  char *pch = sz;
  flag fBold = fFalse, fItalic = fFalse;

  while (*pch) {
    if (fHTML && *pch == ' ' && (*(pch+1) == ' ' || pch == sz))
      fprintf(file, "&nbsp;");
    else if (fHTML && *pch == '\"')
      fprintf(file, "&quot;");
    else if (fHTML && *pch == '&')
      fprintf(file, "&amp;");
    else if (fHTML && (unsigned char)*pch >= 128)
      fprintf(file, "&#%d;", rgwch437[(unsigned char)*pch - 128]); 
    else if (fTextFile && (unsigned char)*pch >= 128)
      fprintf(file, "%c", (char)rgwch437[(unsigned char)*pch - 128]); 
    else if (*pch == '~') {
      if (fHTML)
        fprintf(file, !fBold ? "<b>" : "</b>");
      fBold = !fBold;
    } else if (*pch == '`') {
      if (fHTML)
        fprintf(file, !fItalic ? "<i>" : "</i>");
      fItalic = !fItalic;
    } else
      fprintf(file, "%c", *pch);
    pch++;
  }
}

void PrintLine(char *sz)
{
  Print(sz);
  PrintBr();
}

void PrintCh(char ch, int n)
{
  if (fHTML && ch == ' ') {
    n--;
    while (n > 0) {
      fprintf(file, "&nbsp;");
      n--;
    }
    if (n >= 0)
      fprintf(file, "");
    return;
  }
  while (n > 0) {
    fprintf(file, "%c", ch);
    n--;
  }
}

void PrintLong(char *sz)
{
  char szT[cchSzMax], *pch1, *pch2, *pchT;

  if (fHTML) {
    PrintLine(sz);
    return;
  }
  for (pch1 = pch2 = sz; *pch2; pch2++) {
    if (pch2 - pch1 >= colMax-1) {
      while (pch2 > pch1 && *pch2 != ' ')
        pch2--;
      for (pchT = pch1; pchT < pch2; pchT++)
        szT[pchT - pch1] = *pchT;
      szT[pchT - pch1] = chNull;
      Print(szT);
      fprintf(file, "\n");
      pch1 = pch2 + 1;
    }
  }
  Print(pch1);
  fprintf(file, "\n");
}

void PrintLongBr(char *sz)
{
  PrintLong(sz);
  PrintBr();
}

void PrintIndent(char *sz, flag fExtra)
{
  char *pchW, *pchR, *pch1, *pch2;
  flag fLater = fFalse;

  for (pchW = pchR = sz; *pchW = *pchR; pchW++, pchR++) {
    if (*pchR == '<') {
      *pchW = '*';
      pchR += 2 + (*(pchR+1) == '/');
    }
  }
  for (pch1 = pch2 = sz; *pch2; pch2++) {
    if (pch2 - pch1 >= colMax-3 - (fExtra && fLater)*2) {
      while (pch2 > pch1 && *pch2 != ' ')
        pch2--;
      *pch2 = chNull;
      if (fExtra && fLater)
        printf(" ");
      printf("  %s\n", pch1);
      pch1 = pch2 + 1;
      fLater = fTrue;
    } 
  }
  if (fExtra && fLater)
    printf(" ");
  printf("  %s\n", pch1);
}

void PrintBanner(char *sz)
{
  if (fHTML) {
    fprintf(file, "<hr><h1>%s</h1>\n", sz);
    return;
  }
  PrintCh('*', 79);
  fprintf(file, "\n  %s\n", sz);
  PrintCh('*', 79);
  fprintf(file, "\n\n");
}

void PrintHeading(char *sz)
{
  if (fHTML) {
    fprintf(file, "<b>");
    Print(sz);
    fprintf(file, "</b>");
    PrintBr();
    return;
  }
  PrintLine(sz);
}

void StartColor(int i)
{
  char ch1, ch2;
  int nR, nG, nB;

  if (!fHTML)
    return;
  if (i < 0) {
    i = -i;
    ch1 = ChHex(i >> 4);
    ch2 = ChHex(i & 15);
    fprintf(file, "<font color=\"#%c%c%c%c%c%c\">", ch1, ch2, ch1, ch2, ch1, ch2);
    return;
  }
  if (i > cRay+2) {
    nR = RgbR(i); nG = RgbG(i); nB = RgbB(i);
    fprintf(file, "<font color=\"#%c%c%c%c%c%c\">", ChHex(nR >> 4), ChHex(nR & 15), ChHex(nG >> 4), ChHex(nG & 15), ChHex(nB >> 4), ChHex(nB & 15));
    return;
  }
  fprintf(file, "<font color=\"%s\">", rgszColor[i]);
}

void EndColor()
{
  if (fHTML)
    fprintf(file, "</font>");
}

void StartBkColor(int i)
{
  char ch1, ch2;
  int nR, nG, nB;

  if (!fHTML)
    return;
  if (i < 0) {
    i = -i;
    ch1 = ChHex(i >> 4);
    ch2 = ChHex(i & 15);
    fprintf(file, "<span style=\"background-color:#%c%c%c%c%c%c\">", ch1, ch2, ch1, ch2, ch1, ch2);
    return;
  }
  if (i > cRay+2) {
    nR = RgbR(i); nG = RgbG(i); nB = RgbB(i);
    fprintf(file, "<span style=\"background-color:#%c%c%c%c%c%c\">", ChHex(nR >> 4), ChHex(nR & 15), ChHex(nG >> 4), ChHex(nG & 15), ChHex(nB >> 4), ChHex(nB & 15));
    return;
  }
  fprintf(file, "<span style=\"background-color:%s\">", rgszColor[i]);
}

void EndBkColor()
{
  if (fHTML)
    fprintf(file, "</span>");
}

void StartBar(int i)
{
  if (i == 4)
    i = cRay+2;
  StartBkColor(i);
  StartColor(i);
}

void EndBar()
{
  EndColor();
  EndBkColor();
}

void StartBold()
{
  if (fHTML)
    fprintf(file, "<b>");
}

void EndBold()
{
  if (fHTML)
    fprintf(file, "</b>");
}

void StartFixed()
{
  if (fHTML)
    fprintf(file, "<font face=\"Courier\">\n");
}

void EndFixed()
{
  if (fHTML)
    fprintf(file, "</font>");
}

void StartList()
{
  if (fHTML)
    fprintf(file, "<ul>");
  else
    fprintf(file, "\n");
}

void EndList()
{
  if (fHTML)
    fprintf(file, "</ul>");
  else
    fprintf(file, "\n");
}

void PrintList(int i, char *sz)
{
  if (fHTML)
    fprintf(file, "<li>");
  StartColor(i);
  PrintLong(sz);
  EndColor();
}

int Rnd(int n)
{
  return (rand() & 16383) * n / 16384;
}

flag FFindLatest(int *part, int *ques, int *half)
{
  int i;

  *half = 0;
  for (i = 0; i < cInit; i++)
    if (rgResInit[i][0] == chNull) {
      *part = 1; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cStatement; i++)
    if (rgResS[i].res == 0 || rgResS[i].resf == 0) {
      *part = 2; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cAspire2; i++)
    if (rgResA[i].res == 0) {
      *part = 3; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cAspire2; i++)
    if (rgResA[i].resf == 0) {
      *part = 3; *ques = i; *half = 1; return fFalse;
    }
  for (i = 0; i < cTrait2; i++)
    if (rgResT[i].res == 0) {
      *part = 4; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cTrait2; i++)
    if (rgResT[i].resf == 0) {
      *part = 4; *ques = i; *half = 1; return fFalse;
    }
  *part = 1; *ques = 0;
  return fTrue;
}

void EnsureBetween(int *n, int n1, int n2)
{
  if (*n < n1) {
    printf("ERROR: La variable del archivo guardado %d ('%c') se encuentra por encima del rango entre %d y %d! Asumiendo %d.\n", *n, *n + '0', n1, n2, n1);
    *n = n1;
  }
  if (*n > n2) {
    printf("ERROR: La variable del archivo guardado %d ('%c') se encuentra por encima del rango entre %d y %d! Asumiendo %d.\n", *n, *n + '0', n1, n2, n2);
    *n = n2;
  }
}

void Load(FILE *fileIn)
{
  char sz[cchSzMax], ch;
  int i, n;

  ReadLine(fileIn, sz);
  if (!FEqSz(sz, szDate))
    printf("AVISO: La fecha del archivo guardado (%s) no corresponde con la fecha del programa (%s).\n", sz, szDate);
  for (i = 0; i < cInit; i++) {
    ReadLine(fileIn, sz);
    sprintf(rgResInit[i], "%s", sz);
  }
  for (i = 0; i < cStatement; i++) {
    ch = getc(fileIn); n = (ch - '0'); EnsureBetween(&n, 0, cResponse); rgResS[i].res = n;
    ch = getc(fileIn); n = (ch - '0'); EnsureBetween(&n, 0, cResponse); rgResS[i].resf = n;
  }
  for (i = 0; i < cAspire2; i++) {
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cAspire); rgResA[i].res = n;
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cAspire); rgResA[i].resf = n;
  }
  for (i = 0; i < cTrait2; i++) {
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cTrait); rgResT[i].res = n;
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cTrait); rgResT[i].resf = n;
  }
}

void Save(FILE *fileOut, flag fResults)
{
  FILE *fileSav = file;
  int i;

  file = fileOut;
  PrintLine(szDate);
  for (i = 0; i < cInit; i++)
    PrintLine(rgResInit[i]);
  for (i = 0; i < cStatement; i++) {
    if (fResults && i > 0 && i % ((colMax-1)/2) == 0)
      PrintBr();
    fprintf(file, "%d%d", rgResS[i].res, rgResS[i].resf);
  }
  PrintBr();
  for (i = 0; i < cAspire2; i++)
    fprintf(file, " %d %d", rgResA[i].res, rgResA[i].resf);
  PrintBr();
  for (i = 0; i < cTrait2; i++)
    fprintf(file, " %d %d", rgResT[i].res, rgResT[i].resf);
  PrintBr();
  file = fileSav;
}

flag FResponsedAlready(RESPONSE *rgres, int cres, int n, flag fFormer)
{
  int i;

  for (i = 0; i < cres; i++)
    if ((!fFormer ? rgres[i].res : rgres[i].resf) == n)
      return fTrue;
  return fFalse;
}

int GetString(char sz[cchSzMax])
{
  int cch, i, n;
  char ch = chNull;

  if (fgets(sz, cchSzMax, stdin) == NULL)
    exit(0);
  cch = CchSz(sz);
  while (cch > 0 && sz[cch-1] < ' ')
    cch--;
  sz[cch] = chNull;

  if (sz[0] == '~' && sz[1] != chNull)
    ch = sz[1];
  if (FEqSz(sz, "ayuda")) {
    printf("\n");
    PrintLong("Bienvenido a la ayuda del PIPIII. Escriba su respuesta a cada pregunta y luego presione <Entrar>. Tambi‚n puede escribir cualquiera de los siguientes comandos:");
    printf("  deshacer: Si comete un error al responder, escriba \"deshacer\" para restaurar la pregunta.\n");
    printf("  regresar <#>: Por ejemplo, \"regresar 7\" para retroceder 7 preguntas.\n");
    printf("  saltar <#>: Por ejemplo, \"saltar 3\" para saltar las siguientes 3 preguntas.\n");
    printf("  desplazar <#>: Por ejemplo, \"desplazar 109\" para desplazarse a la pregunta #109.\n");
    printf("  parte <#>: Por ejemplo, \"parte 2\" para desplazarse a la Parte 2: Secci¢n de declaraciones.\n");
    printf("  final: Para desplazarse hasta la primera pregunta no contestada o desplegar los resultados si se ha terminado.\n");
    printf("  reiniciar: Reiniciar la prueba desde cero, borrando todas las respuestas anteriores.\n");
    printf("  ayuda: Mostrar esta lista de ayuda.\n\n");
    return ecRetry;
  }
  if (FPrefixSz(sz, "deshacer") || FPrefixSz(sz, "regresar")) {
    cmdarg = -atoi(sz + 8);
    if (cmdarg >= -1) {
      cmdarg = -1;
      printf("AVISO: Restaurando una pregunta.\n\n");
    } else
      printf("AVISO: Restaurando %d preguntas.\n\n", -cmdarg);
    return ecUndo;
  }
  if (FPrefixSz(sz, "saltar")) {
    cmdarg = atoi(sz + 6);
    if (cmdarg <= 1) {
      cmdarg = 1;
      printf("AVISO: Saltando una pregunta.\n\n");
    } else
      printf("AVISO: Saltando %d preguntas.\n\n", cmdarg);
    return ecUndo;
  }
  if (FPrefixSz(sz, "desplazar")) {
    cmdarg = atoi(sz + 9);
    if (cmdarg <= 1)
      cmdarg = 1;
    printf("AVISO: Desplaz ndose a la pregunta #%d.\n\n", cmdarg);
    cmdarg--;
    return ecGoto;
  }
  if (FPrefixSz(sz, "parte")) {
    cmdarg = atoi(sz + 5);
    if (cmdarg <= 1)
      cmdarg = 1;
    printf("AVISO: Desplaz ndose a la Parte%d.\n\n", cmdarg);
    return ecPart;
  }
  if (FEqSz(sz, "final")) {
    printf("AVISO: Desplaz ndose hasta la primera pregunta no contestada.\n\n");
    return ecLate;
  }
  if (FEqSz(sz, "reiniciar")) {
    printf("AVISO: Reiniciando desde cero!\n\n");
    return ecNew;
  }
  if (ch == 'i') {
    fInfo = !fInfo;
    printf("AVISO: Modo de informaci¢n %s.\n", fInfo ? "ON" : "OFF");
    return ecRetry;
  }
  if (ch == 'z') {
    printf("AVISO: Saltar a la £ltima pregunta de la secci¢n actual.\n\n");
    return ecLast;
  }
  if (ch == 'f') {
    printf("AVISO: Mostrando resultados.\n\n");
    return ecDone;
  }
  if (ch == 'x') {
    printf("AVISO: Saliendo del programa.\n");
    return ecExit;
  }
  if (ch == '?' || ch == '/') {
    for (i = 0; i < cInit; i++)
      sprintf(rgResInit[i], "%s", rgInitDef[i]);
    if (ch == '?') {
      if (sz[2])
        srand(atoi(sz + 2));
      for (i = 0; i < cStatement; i++) {
        rgResS[i].res = Rnd(cResponse) + 1;
        rgResS[i].resf = Rnd(cResponse) + 1;
      }
      for (i = 0; i < cAspire2; i++) {
        do {
          rgResA[i].res = Rnd(cAspire)+1;
        } while (FResponsedAlready(rgResA, i, rgResA[i].res, fFalse));
        do {
          rgResA[i].resf = Rnd(cAspire)+1;
        } while (FResponsedAlready(rgResA, i, rgResA[i].resf, fTrue));
      }
      for (i = 0; i < cTrait2; i++) {
        do {
          rgResT[i].res = Rnd(cTrait)+1;
        } while (FResponsedAlready(rgResT, i, rgResT[i].res, fFalse));
        do {
          rgResT[i].resf = Rnd(cTrait)+1;
        } while (FResponsedAlready(rgResT, i, rgResT[i].resf, fTrue));
      }
      printf("AVISO: Todas las respuestas han sido generadas aleatoriamente. Ingrese \"final\" para mostrar los resultados.\n");
    } else {
      n = sz[2] - '0';
      if (n < 1) n = 1;
      if (n > cResponse) n = cResponse;
      for (i = 0; i < cStatement; i++) {
        rgResS[i].res = n;
        rgResS[i].resf = n;
      }
      for (i = 0; i < cAspire2; i++) {
        rgResA[i].res = cAspire-i;
        rgResA[i].resf = i+1;
      }
      for (i = 0; i < cTrait2; i++) {
        rgResT[i].res = cTrait-i;
        rgResT[i].resf = i+1;
      }
      printf("AVISO: Todas las respuestas han sido configuradas como #%d. Ingrese \"final\" para mostrar los resultados.\n", n);
    }
    return ecRetry;
  }
  return ecNormal;
}

int GetInitResponse(int i)
{
  char sz[cchSzMax];
  int ec;

  printf("Pregunta inicial #%d/%d:\n", i+1, cInit);
  if (rgResInit[i][0])
    printf("AVISO: Ya ha respondido a esta pregunta antes. Usted indic¢: %s\n", rgResInit[i]);
LRetry:
  printf("Ingrese: %s: ", rgInit[i]);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && !rgResInit[i][0]) {
    printf("ERROR: Ingrese algo (incluso si es \"an¢nimo\" o \"desconocido\").\n");
    goto LRetry;
  }
  if (sz[0])
    sprintf(rgResInit[i], "%s", sz);
  else
    printf("AVISO: Manteniendo la respuesta anterior para esta pregunta: %s\n", rgResInit[i]);
  Print("\n");
  return ec;
}

int GetStatementResponse(int s)
{
  char sz[cchSzMax];
  const STATEMENT *ps;
  int i, f, n, n1, n2, nPrev, ec;

  ps = &rgStatement[s];
  printf("Afirmaci¢n #%d/%d:\n", s+1, cStatement);
  sprintf(sz, "%s", ps->sz);
  PrintIndent(sz, fFalse);
  if (fInfo)
    printf("INFO: Rayo %d%s%s, %s, %s\n", ps->ray, (ps->nType & a) > 0 ? "a" : ((ps->nType & b) > 0 ? "b" : ""), (ps->nType & G) > 0 ? " general" : "", ps->fReverse ? "INVERTIDO" : "normal", ps->fSkip ? "SALTAR" : "incluir");
  printf("La afirmaci¢n anterior puede ser:\n");
  for (i = 1; i <= cResponse; i++)
    printf("  %d) %s: %s.\n", i, rgszResponseAbbrev[i], rgszResponse[i]);
  for (f = 0; f <= 1; f++) {
    nPrev = !f ? rgResS[s].res : rgResS[s].resf;
    if (nPrev)
      printf("AVISO: Ya ha respondido a esta afirmaci¢n anteriormente. Usted indic¢: %d (%s)\n", nPrev, rgszResponseAbbrev[nPrev]);
LRetry:
    printf("%s¢mo es que aplica a usted la afirmaci¢n %s (1-%d): ", f == 0 ? "Ingrese c" : "C", f == 0 ? "ACTUALMENTE" : "ANTERIORMENTE en el pasado", cResponse);
    ec = GetString(sz);
    if (ec == ecRetry)
      goto LRetry;
    else if (ec > ecNormal)
      return ec;
    if (!sz[0] && nPrev != 0) {
      printf("AVISO: Manteniendo la respuesta anterior para esta afirmaci¢n: #%d (%s)\n", nPrev, rgszResponseAbbrev[nPrev]);
      goto LDone1;
    }
    n = atoi(sz);
    n1 = n / 10; n2 = n % 10;
    if (f < 1 && n1 >= 1 && n1 <= cResponse && n2 >= 1 && n2 <= cResponse) {
      rgResS[s].res = n1;
      rgResS[s].resf = n2;
      printf("AVISO: Asigne una respuesta %d (%s) a ACTUALMENTE y responda %d (%s) a ANTERIORMENTE.\n", n1, rgszResponseAbbrev[n1], n2, rgszResponseAbbrev[n2]);
      goto LDone2;
    }
    if (n < 1 || n > cResponse) {
      printf("ERROR: Afirmaci¢n inv lida. Introduzca un n£mero del 1 al %d.\n", cResponse);
      goto LRetry;
    }
    if (!f)
      rgResS[s].res = n;
    else
      rgResS[s].resf = n;
LDone1:
    ;
  }
LDone2:
  Print("\n");
  return ec;
}

int GetAspireResponse(int t, flag fFormer)
{
  char sz[cchSzMax];
  const TRAIT *pt;
  int i, n, nPrev, ec;

  printf("Aspiraci¢n m s elevada #%d/%d:\n", t+1, cAspire-1);
  if (t > 0) {
    printf(" Aspiraciones que ya se han clasificado:\n");
    for (i = 0; i < t; i++) {
      n = !fFormer ? rgResA[i].res : rgResA[i].resf;
      if (n < 1)
        continue;
      pt = &rgAspire[n-1];
      sprintf(sz, "#%d (%d): %s", i+1, n, pt->sz);
      PrintIndent(sz, fTrue);
    }
    printf("\n Aspiraciones que a£n NO se han clasificado:\n");
  }
  for (i = 0; i < cAspire; i++) {
    if (FResponsedAlready(rgResA, t, i+1, fFormer))
      continue;
    pt = &rgAspire[i];
    sprintf(sz, "%d: %s", i+1, pt->sz);
    PrintIndent(sz, fTrue);
  }
  nPrev = !fFormer ? rgResA[t].res : rgResA[t].resf;
  if (nPrev)
    printf("AVISO: Ya ha respondido a esta aspiraci¢n anteriormente. Usted indic¢: #%d\n", nPrev);
LRetry:
  printf("Para usted %s, cu l de las aspiraciones arriba indicadas es la m s aplicable (1-%d): ", !fFormer ? "ACTUALMENTE" : "ANTERIORMENTE en el pasado", cAspire);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && nPrev != 0) {
    printf("AVISO: Manteniendo la respuesta anterior para esta aspiraci¢n: #%d\n", nPrev);
    goto LDone;
  }
  n = atoi(sz);
  if (n < 1 || n > cAspire) {
    printf("ERROR: Aspiraci¢n inv lida. Introduzca un n£mero del 1 al %d.\n", cAspire);
    goto LRetry;
  }
  if (FResponsedAlready(rgResA, t, n, fFormer)) {
    printf("ERROR: Aspiraci¢n %d ya seleccionada. Por favor ingrese un nuevo n£mero.\n", n);
    goto LRetry;
  }
  if (!fFormer)
    rgResA[t].res = n;
  else
    rgResA[t].resf = n;
LDone:
  Print("\n");
  return ec;
}

int GetTraitResponse(int t, flag fFormer)
{
  char sz[cchSzMax];
  const TRAIT *pt;
  int i, n, nPrev, ec;

  printf("Rasgos a transformar #%d/%d:\n", t+1, cTrait2);
  if (t > 0) {
    printf(" Rasgos que ya se han clasificado:\n");
    for (i = 0; i < t; i++) {
      n = !fFormer ? rgResT[i].res : rgResT[i].resf;
      if (n < 1)
        continue;
      pt = &rgTrait[n-1];
      sprintf(sz, "#%d (%d): %s", i+1, n, pt->sz);
      PrintIndent(sz, fTrue);
    }
    printf("\n Rasgos que a£n NO se han clasificado:\n");
  }
  for (i = 0; i < cTrait; i++) {
    if (FResponsedAlready(rgResT, t, i+1, fFormer))
      continue;
    pt = &rgTrait[i];
    sprintf(sz, "%d: %s", i+1, pt->sz);
    PrintIndent(sz, fTrue);
  }
  nPrev = !fFormer ? rgResT[t].res : rgResT[t].resf;
  if (nPrev)
    printf("AVISO: Ya ha respondido a este rasgo anteriormente. Usted indic¢: #%d\n", nPrev);
LRetry:
  printf("Para usted %s, cu l de los rasgos arriba indicados es el m s aplicable (1-%d): ", !fFormer ? "ACTUALMENTE" : "ANTERIORMENTE en el pasado", cTrait);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && nPrev != 0) {
    printf("AVISO: Manteniendo la respuesta anterior para este rasgo #%d\n", nPrev);
    goto LDone;
  }
  n = atoi(sz);
  if (n < 1 || n > cTrait) {
    printf("AVISO: Rasgo inv lido. Introduzca un n£mero del 1 al %d.\n", cTrait);
    goto LRetry;
  }
  if (FResponsedAlready(rgResT, t, n, fFormer)) {
    printf("AVISO: Rasgo %d ya seleccionado. Por favor ingrese un nuevo n£mero.\n", n);
    goto LRetry;
  }
  if (!fFormer)
    rgResT[t].res = n;
  else
    rgResT[t].resf = n;
LDone:
  Print("\n");
  return ec;
}

void DoResults()
{
  POWRAY rgPower[cRay+1], rgPowerF[cRay+1], rgPowerMax[cRay+1], *pPower;
  const STATEMENT *ps;
  int rgPattern[cResponse+1][cRay+1][2], i, n, n1, n2, nMax, f, nWhat, t, nTime;
  real r;
  char sz[cchSzMax*2], ch, *pch;

  ClearB(rgPower, sizeof(rgPower));
  ClearB(rgPowerF, sizeof(rgPowerF));
  ClearB(rgPowerMax, sizeof(rgPowerMax));
  ClearB(rgPattern, sizeof(rgPattern));
  for (i = 0; i < cStatement; i++) {
    ps = &rgStatement[i];
    if (ps->fSkip)
      continue;
    for (f = 0; f <= 1; f++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      if (n < 1)
        continue;
      if (ps->fReverse)
        n = cResponse+1 - n;
      n = cResponse - n;
      pPower = !f ? rgPower : rgPowerF;
      if (ps->nType & a) {
        pPower[ps->ray].typea += n;
        if (f) rgPowerMax[ps->ray].typea += cResponse-1;
      }
      if (ps->nType & b) {
        pPower[ps->ray].typeb += n;
        if (f) rgPowerMax[ps->ray].typeb += cResponse-1;
      }
      if (ps->nType & G) {
        pPower[ps->ray].gen += n;
        if (f) rgPowerMax[ps->ray].gen += cResponse-1;
      }
      pPower[ps->ray].tot += n;
      if (f) rgPowerMax[ps->ray].tot += cResponse-1;
    }
  }

  PrintBanner("RESULTADOS del PIP III");
  PrintLine("PIP III: Perfil de Identidad Personal. Versi¢n 3.");
  PrintLine("Copyright (c) 2010-2021 por Michael Robbins y Rick Good. Todos los derechos reservados.");
  sprintf(sz, "Implementaci¢n del programa para PC: %s", szDate);
  PrintLine(sz);
  if (!fHTML)
    PrintLong("Web: https://www.sevenray.org/education/");
  else {
    Print("Para m s informaci¢n visite: ");
    fprintf(file, "<a href=\"https://www.sevenray.org/education/\">https://www.sevenray.org/education/</a><br>\n");
  }
  PrintBr();

  PrintLongBr("El PERFIL DE IDENTIDAD PERSONAL Versi¢n III est  dise¤ado para describir su personalidad y Self Transpersonal o Alma en t‚rminos de ciertas energ¡as sutiles esenciales que condicionan toda la vida en la Tierra. De acuerdo con las ense¤anzas de la forma m s nueva de psicolog¡a, la psicolog¡a esot‚rica, estas energ¡as fundamentales emanan del Sol y los planetas de nuestro sistema solar, de las doce constelaciones del zod¡aco y de las tres constelaciones principales (Sirio, las Pl‚yades y la Osa Mayor). Desde la perspectiva de la psicolog¡a esot‚rica, las energ¡as con las que la humanidad avanzada debe aprender a trabajar lo antes posible son energ¡as que emanan de las siete estrellas principales de la Osa Mayor. La Ense¤anza de la Sabidur¡a Eterna afirma que los cuerpos celestes son mucho m s que objetos celestes inanimados y masivos; por el contrario, de acuerdo con las ciencias esot‚ricas, se cree que los planetas, las estrellas y las constelaciones son los \"cuerpos f¡sicos\" de las grandes Entidades celestiales que tienen una inmensa inteligencia e influencia sobre todos los seres humanos y muchas otras formas de vida de mayor y menor magnitud.");
  PrintLongBr("Si bien existen entidades influyentes dentro del sistema c¢smico local (es decir, muchos planetas no descubiertos y planetoides dentro de nuestro sistema solar, y muchas estrellas vecinas todos los cuales tienen sus influencias \"astro-qu¡micas\" contribuyes), la forma actual de el PERFIL DE IDENTIDAD PERSONAL III se ocupa £nicamente de las influencias energ‚ticas de la m s importante de estas Entidades: las siete estrellas principales (Entidades) de la Osa Mayor, de las que emanan las siete energ¡as conocidas como los Siete Rayos.");

  PrintBanner("INFORMACIÓN DEMOGRÁFICA");
  for (i = 0; i < cInit; i++) {
    sprintf(sz, "%s: %s", rgInit2[i], rgResInit[i]);
    PrintLine(sz);
  }
  PrintBr();

  for (nWhat = 0; nWhat < 3; nWhat++) {
    switch (nWhat) {
    case 0: pch = "RAYOS"; break;
    case 1: pch = "TIPOS DE RAYO"; break;
    case 2: pch = "LÍNEAS DE RAYO"; break;
    }
    PrintBanner(pch);

  if (nWhat == 0) {
  } else if (nWhat == 1) {
    PrintLongBr("Aunque probablemente existen \"siete\" (sub)tipos relacionados con cada uno de los Siete Rayos, PIP III enfatiza solo dos (sub)tipos principales correlacionados con sugerencias que el Maestro Tibetano ha dado en varios de Sus libros. En algunos casos, la distinci¢n entre TIPO A y TIPO B se relaciona con el grado de \"abstracci¢n\" o de \"concreci¢n\" como en los casos de los Tipos 3A y 3B, y los Tipos 5A y 5B, pero no en todas las distinciones de tipos. A efectos pr cticos, podr¡amos pensar en los dos (sub)tipos de rayos que se ofrecen como dos (sub)tipos que \"aparecen con frecuencia\" en relaci¢n con un rayo en particular.");
    PrintLongBr("Debe entenderse que es posible que usted obtenga una puntuaci¢n alta en un `(sub)tipo` de rayo determinado y baja en el otro `(sub)tipo`. Muy a menudo, es posible que las personas del mismo rayo no se reconozcan entre s¡ porque son ejemplos de diferentes (sub)tipos. El Maestro DK, por ejemplo, ha dado tres (sub)tipos en relaci¢n con el segundo rayo (especialmente el Alma de segundo rayo). Aquellos en los aspectos de Sabidur¡a o Amor del segundo rayo pueden no reconocerse mutuamente de forma inmediata como pertenecientes al `mismo rayo`.");
    PrintLongBr("Algunas personas que utilizan el PIP han encontrado que los GRAFICOS DE TIPOS DE RAYOS son a£n m s indicativos acerca de sus rayos (i.e., su F¢rmula de Rayo) que los GRAFICOS DE RAYOS (considerando los rayos como siete grandes divisiones de energ¡a). Una cosa es segura: si obtiene una puntuaci¢n muy alta en uno de los tipos de rayo (aunque no en el otro tipo), ese rayo es probablemente fuerte en su sistema energ‚tico. La £nica excepci¢n podr¡a ser una puntuaci¢n alta en R1A que parece evocar una puntuaci¢n alta incluso de aquellos que `no` tienen un R1 fuerte, pero `~desear¡an~` tenerlo.");
    PrintLong("Este gr fico tiene catorce escalas de barras, que representan dos tipos principales para cada rayo. Para obtener una descripci¢n verdaderamente detallada de los tipos, por favor, estudie `Tapestry of the Gods` - Vols. I y II. Las divisiones de los tipos son las siguientes:");
    StartList();
    PrintList(1, "~R1A:~ El Tipo Fuerte y Firme del primer rayo, muy correlacionado con la influencia de Vulcano y Saturno. Esta escala generalmente tiende a ser `demasiado elevada` en el caso de muchas personas. Cuando es aut‚ntico, indica la capacidad de `mantenerse firme` a pesar de la embestida de las fuerzas opuestas.");
    PrintList(1, "~R1B:~ El Tipo Destructivo del primer rayo, muy correlacionado con la influencia de Plut¢n y tal vez de Marte. Es raro que aquellos que eligen R1A por razones de \"deseabilidad social\" (o 'deseabilidad esot‚rica') elijan R1B por razones similares. Cuando R1B es alto, es casi seguro que indica la presencia del primer rayo en el sistema energ‚tico y tal vez de un Alma de primer rayo.");
    PrintList(2, "~R2A:~ El Tipo Amor Puro de R2. Aquellos que expresan fuertemente el Aspecto Cr¡stico del segundo rayo obtienen una puntuaci¢n alta en este Tipo de Rayo. Este tipo puede faltar un poco al principio en aquellos cuyo enfoque de la espiritualidad implica, estrictamente hablando, la b£squeda de la Sabidur¡a.");
    PrintList(2, "~R2B:~ El Tipo Sabidur¡a de R2. Aquellos que expresan fuertemente el aspecto B£ddhico del segundo rayo obtienen una puntuaci¢n alta en este Tipo de Rayo. Este tipo puede carecer un poco de expresi¢n de amor, enfatizando en su lugar la sabidur¡a, aunque en nuestro sistema solar, el Amor y la Sabidur¡a dif¡cilmente pueden separarse entre s¡.");
    PrintList(3, "~R3A:~ El Tipo Pensamiento Abstracto de R3: Este tipo tiende a ser elevado en aquellos a los que les gusta pensar abstractamente en relaci¢n con temas filos¢ficos, matem ticos, econ¢micos o, en general, especulativos. En tales individuos, el contacto con el  tomo permanente man sico tiende a activarse a trav‚s de un aspecto del antahkarana (el Puente Arco Iris).");
    PrintList(3, "~R3B:~ El Tipo Comercial y Activo de R3. El tipo 3B est  m s centrado materialmente que el tipo 3A, y se caracteriza por una vida de gran actividad externa, tanto en el plano f¡sico como en la mente concreta.");
    PrintList(4, "~R4A:~ El Tipo Armon¡a y Belleza de R4. El tipo R4A est  m s inclinado a trabajar sensible y est‚ticamente hacia la armon¡a que a participar en conflictos.");
    PrintList(4, "~R4B:~ El Tipo Dram tico y Conflictivo de R4. El tipo R4B vive una vida de muchas luchas entre fuerzas opuestas, tanto dentro del medio ambiente como dentro del sistema energ‚tico humano. La tendencia final puede dirigirse hacia el logro de la armon¡a, pero no sin un conflicto dram tico considerable como precursor de la armon¡a que se debe lograr.");
    PrintList(5, "~R5A:~ El Tipo Anal¡tico y Discriminativo de R5. El tipo R5A tiene un enfoque agudo y anal¡tico para muchos asuntos. Es conocido por su dominio de los detalles f cticos y los temas especializados.");
    PrintList(5, "~R5B:~ El Tipo Mec nico e Inventivo de R5. El tipo R5B se inclina a hacer una aplicaci¢n pr ctica de todo lo que sabe, especialmente en el campo de la mec nica, la electr¢nica y, en general, en todos los campos t‚cnicos del conocimiento aplicado.");
    PrintList(6, "~R6A:~ El Tipo Devoto de R6. El tipo 6A expresa el tipo m s amable del sexto rayo, m s expresivo de la energ¡a neptuniana. Representa un tipo de energ¡a que se inclina hacia un profundo apego a los objetos de devoci¢n y al idealismo trascendental.");
    PrintList(6, "~R6B:~ El Tipo Ardoroso y Ferviente de R6. El tipo 6B expresa fuerzas m s correlacionadas con el planeta Marte que Neptuno. Sa naturaleza es fiera y a menudo destructiva cuando se desaf¡an sus ideales. Este tipo de energ¡a del sexto rayo a menudo puede estar involucrado en la persecuci¢n de aquellos que no comparten sus ideales, visiones y credos y tiende al fanatismo.");
    PrintList(7, "~R7A:~ El Tipo Altamente Organizado de R7. El tipo 7A a menudo se correlaciona con una personalidad de s‚ptimo rayo. El amor por la apariencia, el patr¢n, la rutina y la disposici¢n correctos es pronunciado. El poder de Saturno es prominente.");
    PrintList(7, "~R7B:~ El Tipo Transformador y Renovador de R7. Es m s probable que el tipo 7B se correlacione con el `Alma` de s‚ptimo rayo que con la personalidad del s‚ptimo rayo. Se considera que la `re`novaci¢n y la reorganizaci¢n son m s importantes que el mantenimiento de un orden estricto. El poder de Urano (un planeta en gran parte del s‚ptimo rayo) es prominente.");
    EndList();
  } else if (nWhat == 2)
    PrintLongBr("Aunque existen Siete Rayos, est n divididos en dos corrientes o l¡neas de energ¡a principales. La primera l¡nea deriva de R1 e incluye R3, R5 y R7; estos cuatro son los llamados rayos de \"l¡nea dura\". La segunda l¡nea (la \"l¡nea suave\") deriva de R2 e incluye R4 y R6. A menudo es posible obtener una impresi¢n general de uno mismo o de otro individuo simplemente consultando la elevaci¢n relativa de las dos l¡neas de rayos: la \"l¡nea dura\" en rojo y la \"l¡nea suave\" en azul. La l¡nea dura est  m s relacionada con la forma y la l¡nea blanda con la vida dentro de la forma. El equilibrio general de las dos l¡neas de rayos se puede evaluar razonablemente consultando estos gr ficos. Se puede ver la tendencia general de las dos l¡neas a aumentar o disminuir. Tambi‚n puede decidir de manera general si debe trabajar para lograr un mejor equilibrio entre ambas.");

  for (nTime = 0; nTime < 5; nTime++) {
    StartBold();
    Print(" ");
    switch (nWhat) {
    case 0: Print("Rayos"); break;
    case 1: Print("Tipos de Rayo"); break;
    case 2: Print("L¡neas de Rayo"); break;
    }
    Print(" ");
    switch (nTime) {
    case 0: Print("ACTUALMENTE"); break;
    case 1: Print("ANTERIORMENTE"); break;
    case 2: Print("ACTUALMENTE y ANTERIORMENTE combinados"); break;
    case 3: Print("ANTERIORMENTE frente a ACTUALMENTE comparado"); break;
    case 4: Print("Diferencia de ANTERIORMENTE a ACTUALMENTE"); break;
    }
    Print(":");
    EndBold();
    PrintBr();

if (nWhat == 0) {
  if (nTime == 0) {
    PrintLongBr("Este gr fico es uno de los m s importantes. Vemos en ‚l siete elevaciones para los Siete Rayos tal como deben entenderse `en general`, sin tomar en consideraci¢n los Tipos de Rayo, es decir, los subtipos de cualquier rayo en particular (y puede haber, incluso, siete subtipos que se identificar n m s adelante). Las afirmaciones correlacionadas con esta tabla est n destinadas a probar las cualidades que demostrar n todas las personas que expresan fuertemente un rayo, independientemente del Tipo de Rayo.");
    PrintLongBr("Para obtener una comprensi¢n de sus rayos, RAYOS ACTUALMENTE `no` debe estudiarse de forma aislada. Debe estudiarse junto con los Tipos de Rayo para obtener la mejor visi¢n general. Adem s, es indispensable un estudio de su carta astral, aunque esta es una consideraci¢n m s avanzada.");
    PrintLongBr("RAYOS ACTUALMENTE intenta puntuar su respuesta a las tendencias de los rayos `en el momento presente`, el momento en que toma el PIP III. Siempre hay un momento presente y una configuraci¢n de rayos razonablemente correcta para ese momento. Si bien el rayo del Alma puede cambiar solo una vez (o tal vez, como mucho, dos veces) a lo largo de la historia de su encarnaci¢n humana de millones de a¤os, su personalidad puede comenzar a cambiar en la presente encarnaci¢n para que hacia el £ltimo tercio de su vida pueda evidenciar signos del rayo de la personalidad que tendr s en tu pr¢xima encarnaci¢n. Esto tambi‚n es cierto para los rayos de los veh¡culos de su personalidad. Entonces, debe entenderse que su F¢rmula de Rayo est  en un estado de flujo durante su presente encarnaci¢n. Y cada uno de los veh¡culos en su sistema de energ¡a tambi‚n tiene subrayos.");
    PrintLongBr("Es mejor estudiar RAYOS ACTUALMENTE para dar una visi¢n general de la fuerza actual de los Siete Rayos tal como se manifiestan `generalmente` en su sistema de energ¡a en este momento. Sin embargo, no debe suponer que el rayo m s alto es necesariamente el rayo de su Alma. Puede ser el rayo de su personalidad o incluso el rayo de su mente si est  muy enfocado mentalmente. Tambi‚n hay que decir que \"el pensamiento idealista\" a veces afecta la elevaci¢n de una barra en particular en el gr fico. Por ejemplo, si tiene un profundo deseo de ser fuerte y firme, puede responder a las afirmaciones del Rayo Uno como si fuera m s fuerte y firme de lo que realmente es. La otra escala general, RAYOS ANTERIORMENTE, est  destinada a ayudarlo a diferenciar entre c¢mo \"ha sido\" y c¢mo \"realmente es en la actualidad\".");
    PrintLongBr("Los factores astrol¢gicos tambi‚n pueden afectar la elevaci¢n de los rayos. Los Siete Rayos se transmiten a trav‚s de varios signos astrol¢gicos y planetas, por lo que un tipo particular de ‚nfasis astrol¢gico en el momento presente puede estimular o deprimir la elevaci¢n de cierto rayo en RAYOS ACTUALMENTE independientemente de su F¢rmula de Rayo. Por eso es muy importante comprender la astrolog¡a esot‚rica cuando se intenta obtener una evaluaci¢n precisa de su Carta de Rayos, su F¢rmula de Rayos. Para una imagen m s completa, definitivamente se debe considerar su carta astrol¢gica junto con los hallazgos del PIP III.");
  } else if (nTime == 1) {
    PrintLongBr("Este gr fico est  destinado a revelar c¢mo los Siete Rayos se expresaron a trav‚s de su sistema de energ¡a `en alg£n momento pasado`. No buscamos la evaluaci¢n de `cualquier momento particular` de su vida, sino m s bien una especie de \"promedio subjetivo\" que contrasta el c¢mo cree que \"sol¡a ser\" en comparaci¢n con el \"c¢mo cree que es ahora\". En pocas palabras, las elevaciones de los rayos en el gr fico est n destinadas a mostrarle d¢nde ha `estado`, en comparaci¢n con d¢nde est  `actualmente`. Ver , pues, qu‚ rayos, a su juicio, se han vuelto m s fuertes, se han mantenido m s o menos iguales, o se han reducido en la fuerza de su expresi¢n. Por lo tanto, al comparar RAYOS ANTERIORMENTE con RAYOS ACTUALMENTE, puede ver importantes tendencias de rayos en su vida y tambi‚n, quiz s, identificar tendencias astrol¢gicas subyacentes.");
  } else if (nTime == 2) {
    PrintLongBr("Este gr fico es £til porque muestra c¢mo las tendencias del pasado (ANTERIORMENTE) se fusionan y mezclan con las tendencias del presente (ACTUALMENTE). Nuestras respuestas y comportamientos presentes tienen muchos `remanentes` del pasado; en nuestra etapa de evoluci¢n, rara vez estamos libres de tendencias pasadas. Aunque son del pasado, viven en el presente. Por lo tanto, es interesante ver el resultado de combinar tendencias pasadas con evaluaciones actuales de actitudes, respuestas y comportamientos.");
  } else if (nTime == 3) {
    PrintLongBr("Este gr fico es £til porque muestra comparativamente las elevaciones de los Rayos Globales de ANTERIORMENTE a ACTUALMENTE. Esta es otra forma de ver importantes ~tendencias de rayos globales~ en esta encarnaci¢n en particular.");
  } else if (nTime == 4) {
    PrintLongBr("Este gr fico es £til para ver (seg£n el porcentaje de aumento o disminuci¢n) el grado en que la intensidad de los rayos ha cambiado con el tiempo. La fuerza relativa de los rayos cambia constantemente durante el curso de una encarnaci¢n dada y de una encarnaci¢n a otra. Todos tenemos `~todos los rayos~`, sin embargo, el `‚nfasis` cambia, transformando as¡ el ‚nfasis de cualquier rayo en relaci¢n con los dem s. Queremos saber si hay una cierta tendencia en los cambios de rayos que estamos experimentando o hemos sufrido. Tal vez tambi‚n se pueda discernir la tendencia del rayo de la pr¢xima encarnaci¢n. Como todos los disc¡pulos, es posible que deseemos tener m s y m s virtudes de los rayos (aunque no nos sintamos particularmente relacionados con ellos), pero se requiere una estimaci¢n realista, no basada simplemente en pensamiento idealista o en el 'idealismo de los rayos'.");
  }
}

    if (nWhat == 2 && nTime == 0)
      for (n = 1; n <= 2; n++)
        for (i = n + 2; i <= cRay; i += 2) {
          rgPower[n].tot += rgPower[i].tot;
          rgPowerF[n].tot += rgPowerF[i].tot;
          rgPowerMax[n].tot += rgPowerMax[i].tot;
        }
    for (i = 1; i <= cRay; i++) {
      if (i > 2 && nWhat == 2)
        break;
      for (f = 0; f <= 1; f++) {
        if (f && nTime != 3)
          continue;
        for (t = 0; t <= 1; t++) {
        if (t && nWhat != 1)
          continue;
        if (nWhat < 1) {
          n1 = rgPowerF[i].gen;
          n2 = rgPower[i].gen;
          nMax = rgPowerMax[i].gen;
        } else if (nWhat > 1) {
          n1 = rgPowerF[i].tot;
          n2 = rgPower[i].tot;
          nMax = rgPowerMax[i].tot;
        } else if (!t) {
          n1 = rgPowerF[i].typea;
          n2 = rgPower[i].typea;
          nMax = rgPowerMax[i].typea;
        } else {
          n1 = rgPowerF[i].typeb;
          n2 = rgPower[i].typeb;
          nMax = rgPowerMax[i].typeb;
        }
        n = nTime == 0 ? n2 :
          (nTime == 1 ? n1 :
          (nTime == 2 ? n1 + n2 :
          (nTime == 4 ? n2 - n1 :
          (f ? n2 : n1))));
        nMax *= (nTime == 2 ? 2 : 1);
        r = (nMax == 0) ? 0.0 : (real)n / (real)nMax * 100.0;
        StartColor(i);
        StartFixed();
        if (nWhat < 2)
          fprintf(file, "Rayo %d%s", i, nWhat != 1 ? "" : (t ? "B" : "A"));
        else {
          sprintf(sz, "%s L¡neas de Rayo", i <= 1 ? "Dura " : "Suave");
          Print(sz);
        }
        sprintf(sz, "%s:", nTime != 3 ? "" : (f ? " actualmente  " : " anteriormente"));
        Print(sz);
        if (fInfo) {
          sprintf(sz, "%4d/%2d =", n, nMax);
          Print(sz);
        }
        sprintf(sz, "%6.2f%%: ", r);
        Print(sz);
        EndFixed();
        n = n1 = fHTML ? (int)(r * 1.0) : (int)r / 2;
        if (nTime == 4) {
          n2 = n < 0 ? (fHTML ? 50 : 25) + n : (fHTML ? 50 : 25);
          StartColor(cRay+1);
          PrintCh(' ', n2);
          EndColor();
          if (n >= 0) {
            StartBar(0);
            Print(fHTML ? "" : "|");
            EndBar();
          }
          n = NAbs(n);
        }
        ch = fHTML ? ' ' : (FOdd(i) ? 'x' : 'o');
        if (!fHTML || i != 6) {
          StartBar(i);
          PrintCh(ch, n);
          EndBar();
        } else {
          for (n2 = 0; n2 < n; n2++) {
            StartBar(BlendColor(Rgb(255, 62, 223), Rgb(0, 58, 255), n2, n-1));
            fprintf(file, "&nbsp;");
            EndBar();
          }
        }
        if (nTime == 4 && n1 < 0) {
          StartBar(0);
          Print(fHTML ? "" : "|");
          EndBar();
        }
        EndColor();
        PrintBr();
        }
      }
    }
    PrintBr();
  }
  }

  PrintBanner("ASPIRACIONES MÁS ELEVADAS");
  PrintLong("Si bien a menudo se cree que las aspiraciones se originan en la personalidad, en realidad, son estimuladas por el poder magn‚tico del Alma que atrae a la personalidad \"hacia arriba\". Esta tabulaci¢n le revelar  sus aspiraciones m s elevadas seg£n su Tipo de Rayo. Las metas que ocupen el rango m s alto tendr n mucho que ver con los rayos de su Alma y personalidad. Aquellas que usted clasifique m s bajo pueden correlacionarse con rayos que no se encuentran en su sistema energ‚tico. A veces, una aspiraci¢n elevada tiene que ver con las cualidades idealizadas pero que en realidad no forman parte del sistema energ‚tico actual. En este caso las aspiraciones son `compensatorias`. A veces es posible que ya no aspire a los objetivos a los que sol¡a aspirar, al menos no en la misma medida que en el pasado. En cualquier caso, la clasificaci¢n de sus aspiraciones deber¡a ayudar a confirmar su F¢rmula de Rayo y debe correlacionarse con los dem s aspectos del PIP III.");
  StartList();
  for (i = 0; i < cAspire; i++) {
    sprintf(sz, "Aspiraci¢n #%d (Rayo %d): %s", i+1, rgAspire[i].ray, rgAspire[i].sz);
    PrintList(rgAspire[i].ray, sz);
  }
  EndList();
  StartFixed();
  StartBold();
  PrintLine("CATEGORIA     ACTUALMENTE          ANTERIORMENTE");
  EndBold();
  EndFixed();
  for (i = 0; i < cAspire; i++) {
    if (i < cAspire2) {
      n1 = rgResA[i].res;
      n2 = rgResA[i].resf;
    } else {
      for (f = 0; f <= 1; f++) {
        n = 0;
        for (t = 0; t < cAspire2; t++)
          n += (!f ? rgResA[t].res : rgResA[t].resf);
        n = cAspire * (cAspire+1) / 2 - n;
        if (!f)
          n1 = (n <= cAspire ? n : 0);
        else
          n2 = (n <= cAspire ? n : 0);
      }
    }
    if (n1 < 1 || n2 < 1)
      continue;
    StartFixed();
    fprintf(file, "#%2d: ", i+1);
    StartColor(rgAspire[n1-1].ray);
    Print("Aspiraci¢n ");
    fprintf(file, "#%2d (Rayo %d)", n1, rgAspire[n1-1].ray);
    EndColor();
    Print(", ");
    StartColor(rgAspire[n2-1].ray);
    Print("Aspiraci¢n ");
    fprintf(file, "#%2d (Rayo %d)", n2, rgAspire[n2-1].ray);
    EndColor();
    EndFixed();
    PrintBr();
  }
  PrintBr();

  PrintBanner("RASGOS A TRANSFORMAR");
  PrintLong("En el ocultismo, estos rasgos se llaman \"espejismos\" e \"ilusiones\". Se relacionan con las cualidades negativas que los individuos expresan cuando est n fuertemente influenciados por ciertos rayos particulares, pero no pueden controlar o dirigir adecuadamente la expresi¢n de dichos rayos. Las energ¡as no son ni buenas ni malas; todas dependen de la condici¢n de la forma a trav‚s de la cual trabajan. Todos tenemos rasgos negativos que deber¡an transformarse en rasgos positivos. Estas expresiones negativas deber¡an correlacionarse con las energ¡as de rayos que se encuentran en su Carta de Rayos actual, o tal vez en la indicada como anteriormente, por ejemplo, en una encarnaci¢n anterior. Al estudiar sus \"Rasgos a transformar\" m s destacados, puede recopilar consejos importantes sobre la presencia de ciertos rayos en su F¢rmula de Rayos. Tal vez, podr  observar qu‚ algunos antiguos \"Rasgos a transformar\" ya no le est n causando dificultades. En general, tambi‚n podr  recibir pistas sobre la forma en que puede mejorar su car cter.");
  StartList();
  for (i = 0; i < cTrait; i++) {
    sprintf(sz, "Rasgo #%d (Rayo %d): %s", i+1, rgTrait[i].ray, rgTrait[i].sz);
    PrintList(rgTrait[i].ray, sz);
  }
  EndList();
  StartFixed();
  StartBold();
  PrintLine("CATEGORIA  ACTUALMENTE    ANTERIORMENTE");
  EndBold();
  EndFixed();
  for (i = 0; i < cTrait2; i++) {
    n1 = rgResT[i].res;
    n2 = rgResT[i].resf;
    if (n1 < 1 || n2 < 1)
      continue;
    StartFixed();
    fprintf(file, "#%d: ", i+1);
    StartColor(rgTrait[n1-1].ray);
    fprintf(file, "Rasgo #%2d (Rayo %d)", n1, rgTrait[n1-1].ray);
    EndColor();
    Print(", ");
    StartColor(rgTrait[n2-1].ray);
    fprintf(file, "Rasgo #%2d (Rayo %d)", n2, rgTrait[n2-1].ray);
    EndColor();
    EndFixed();
    PrintBr();
  }
  PrintBr();

  PrintBanner("PATRONES DE RESPUESTA");
  PrintLongBr("Hay un gran valor en esta tabulaci¢n. Se indican tendencias de respuesta definidas y estas tendencias pueden correlacionarse con los rayos. Por ejemplo, aquellos con un R5 alto en su sistema de energ¡a, pueden ser altamente discriminatorios y rara vez usan la categor¡a Definitivamente Verdadero, porque siempre pueden encontrar excepciones. Las elevaciones generales de sus columnas de rayos tienden a ser m s bajas que las elevaciones de algunos otros Tipos de Rayo. Con los tipos R6, tanto la categor¡a Definitivamente Verdadero como Definitivamente Falso pueden enfatizarse fuertemente, porque tienden a ver la vida en t‚rminos de extremos. Las elevaciones de rayos para los tipos R6 tienden a ser m s altas de lo normal, tal vez porque `no` son tan discriminantes. Tambi‚n est n en gran medida \"concentrados\", lo que significa que las categor¡as DV y GV tendr n un peso considerable. Cuando se trata de tipos R1, pueden verse a s¡ mismos como apropiados de todos los rayos y, por lo tanto, de nuevo, podemos encontrar elevaciones altas de rayo. Los tipos R4 pueden tener dificultades para ver las cosas de una manera u otra, por lo que las respuestas que son \"M s Verdadero que Falso\", \"En el Medio\" o \"M s Falso que Verdadero\" se pueden encontrar en abundancia. El Patr¢n de Respuesta de Rayo tambi‚n se puede utilizar para detectar patrones de extroversi¢n e introversi¢n, con elevaciones m s altas que indican la primera (extroversi¢n) y elevaciones relativamente m s bajas que la segunda (introversi¢n). Todo esto quiere decir que el Patr¢n de Respuesta a los rayos debe estudiarse por las pistas que pueda revelar con respecto a los rayos predominantes. Se est n llevando a cabo m s an lisis sobre este tipo de tabulaci¢n.");
  PrintLongBr("Tambi‚n hay que decir que cuando un rayo tiene un gran n£mero en la categor¡a Definitivamente Verdadero, se puede presumir que ese rayo es prominente en el sistema energ‚tico. Si el mismo rayo que tiene muchas respuestas definitivamente verdaderas tambi‚n tiene algunas respuestas en la categor¡a Definitivamente Falso, la causa generalmente se encuentra en la diferencia entre los tipos de rayos, que a menudo pueden ser de naturaleza muy contrastante.");
  for (f = 0; f <= 1; f++)
    for (i = 0; i < cStatement; i++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      rgPattern[n][rgStatement[i].ray][f]++;
      rgPattern[n][0][f]++;
    }
  for (f = 0; f <= 1; f++) {
    PrintHeading(!f ? "ACTUALMENTE" : "ANTERIORMENTE");
    StartFixed();
    PrintCh(' ', 7);
    for (n = 1; n <= cRay; n++) {
      StartColor(n);
      sprintf(sz, " R%d", n);
      Print(sz);
      EndColor();
    }
    Print(" Total");
    EndFixed();
    PrintBr();
    for (i = 1; i <= cResponse; i++) {
      StartFixed();
      fprintf(file, "%d) %s:", i, rgszResponseAbbrev[i]);
      for (n = 1; n <= cRay; n++) {
        StartColor(n);
        sprintf(sz, "%3d", rgPattern[i][n][f]);
        Print(sz);
        EndColor();
      }
      n = rgPattern[i][0][f];
      sprintf(sz, "%4d", n);
      Print(sz);
      if (fInfo)
        fprintf(file, "/%d", cStatement);
      fprintf(file, "=%5.2f%%", (real)n / (real)cStatement * 100.0);
      Print(": ");
      EndFixed();
      n = n * (fHTML ? 100 : 50) / cStatement;
      StartColor(-(i-1) * (192/(cResponse-1)));
      StartBkColor(-(i-1) * (192/(cResponse-1)));
      PrintCh(fHTML ? ' ' : '+', n);
      EndBkColor();
      EndColor();
      PrintBr();
    }
    PrintBr();
  }

  PrintBanner("RESPUESTAS A LAS AFIRMACIONES");
  PrintLong("Esta es una tabulaci¢n completa que le permite encontrar r pidamente su respuesta a `cualquier` afirmaci¢n en el PIP III. Puede ver f cilmente c¢mo ha respondido a cualquier afirmaci¢n hecha con respecto al pasado (ANTERIORMENTE) y al presente (ACTUALMENTE). Al reflexionar sobre la afirmaci¢n en cuesti¢n (de una copia del PIP III), puede revisar c¢mo se sinti¢ y pens¢ sobre esa afirmaci¢n en particular cuando tom¢ el PIP III y si todav¡a piensa y se siente de esa manera.");
  StartList();
  for (i = 0; i < cStatement; i++) {
    sprintf(sz, "Afirmaci¢n #%d (Rayo %d%s): %s", i+1, rgStatement[i].ray, (rgStatement[i].nType & a) > 0 ? "a" : ((rgStatement[i].nType & b) > 0 ? "b" : ""), rgStatement[i].sz);
    PrintList(rgStatement[i].ray, sz);
  }
  EndList();
  StartFixed();
  for (i = 1; i <= cResponse; i++) {
    sprintf(sz, "%d) %s = %s.", i, rgszResponseAbbrev[i], rgszResponse[i]);
    PrintLine(sz);
  }
  EndFixed();
  PrintBr();
  for (f = 0; f <= 1; f++) {
    PrintHeading(!f ? "ACTUALMENTE" : "ANTERIORMENTE");
    StartFixed();
    for (i = 0; i < cStatement; i++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      StartColor(rgStatement[i].ray);
      sprintf(sz, "%4d:%s", i+1, rgszResponseAbbrev[n]);
      Print(sz);
      EndColor();
      if (i >= cStatement-1 || (i+1) % 10 == 0)
        PrintBr();
    }
    EndFixed();
    PrintBr();
    for (i = 1; i <= cResponse; i++) {
      fprintf(file, "%d) %s:", i, rgszResponseAbbrev[i]);
      for (t = 0; t < cStatement; t++) {
        n = !f ? rgResS[t].res : rgResS[t].resf;
        if (n == i) {
          StartColor(rgStatement[t].ray);
          sprintf(sz, " %d", t+1);
          Print(sz);
          EndColor();
        }
      }
      PrintBr();
    }
    if (!f)
      PrintBr();
  }
  PrintBr();

  PrintBanner("FIN DE LOS RESULTADOS");
  sprintf(sz, "Gracias por realizar la prueba PIP III. Cuando haya completado la prueba, deber¡a haber aparecido una ventana de correo electr¢nico con sus resultados. T¢mese un momento para enviarnos este mensaje a %sAstara@msn.com%s. Su privacidad es importante, y estos resultados se mantienen confidenciales y solo se utilizan con fines de an lisis estad¡stico e investigaci¢n, y para guiar futuras versiones de la prueba. Si no apareci¢ una ventana de correo electr¢nico, env¡enos un correo electr¢nico manualmente con este archivo de resultados, o simplemente copie y pegue el siguiente contenido:", fHTML ? "<a href=\"mailto:Astara@msn.com\">" : "", fHTML ? "</a>" : "");
  if (!fHTML)
    PrintLongBr(sz);
  else {
    Print(sz);
    fprintf(file, "<br>\n", sz);
  }
  if (file != stdout) {
    if (fHTML)
      fprintf(file, "<blockquote>");
    StartFixed();
    StartColor(-128);
    PrintLong("[Empieza la secci¢n de texto para enviar por correo electr¢nico.]");
    Save(file, fTrue);
    PrintLong("[Finaliza la secci¢n del texto para enviar por correo electr¢nico.]");
    EndColor();
    EndFixed();
    if (fHTML)
      fprintf(file, "</blockquote>");
    else
      PrintBr();
  }
  sprintf(sz, "El PIP III se ofrece de forma gratuita y est  respaldado por contribuciones voluntarias. Si desea contribuir y ayudar a continuar el trabajo, visite %shttps://www.sevenray.org/donate.html%s. ­Gracias!", fHTML ? "<a href=\"https://www.sevenray.org/donate.html\">" : "", fHTML ? "</a>" : "");
  if (!fHTML)
    PrintLong(sz);
  else
    fprintf(file, "%s<br>\n", sz);
}

int main(int argc, char **argv)
{
  const STATEMENT *ps;
  const TRAIT *pt;
  int iPart, i, iMax, f, ec, n;
  char sz[cchSzMax*10], szMail[cchSzMax*11], *pch, *pch2, ch;
  FILE *fileIn;

  file = stdout;
  if (argc > 1) {
    fileIn = fopen(argv[1], "r");
    if (fileIn == NULL) {

    for (i = 0; i < cInit; i++)
      printf("#%d: %s\n", i+1, rgInit2[i]);
    printf("\n");
    for (i = 0; i < cStatement; i++) {
      ps = &rgStatement[i];
      printf("#%d: Ray %d%s%s, %s, %s: %s\n", i+1, ps->ray, (ps->nType & a) > 0 ? "a" : ((ps->nType & b) > 0 ? "b" : ""), (ps->nType & G) > 0 ? " general" : "", ps->fReverse ? "REVERTIR" : "normal", ps->fSkip ? "SALTAR" : "Incluir", ps->sz);
    }
    printf("\n");
    for (i = 0; i < cAspire; i++) {
      pt = &rgAspire[i];
      printf("#%d: Ray %d: %s\n", i+1, pt->ray, pt->sz);
    }
    printf("\n");
    for (i = 0; i < cTrait; i++) {
      pt = &rgTrait[i];
      printf("#%d: Ray %d: %s\n", i+1, pt->ray, pt->sz);
    }
    return 0;

    } else {

    Load(fileIn);
    fclose(fileIn);
    goto LLoaded;

    }
  }

LNew:
  ClearB(rgResInit, sizeof(rgResInit));
  ClearB(rgResS, sizeof(rgResS));
  ClearB(rgResA, sizeof(rgResA));
  ClearB(rgResT, sizeof(rgResT));

  printf("Bienvenido al PIP III: Perfil de Identidad Personal. Versi¢n 3.\n");
  printf("Copyright (c) 2010-2021 por Michael Robbins y Rick Good. Todos los derechos\nreservados.\n");
  printf("Implementaci¢n del programa para PC: %s por Walter Pullen\n(%s). Traducci¢n al castellano: Luis Alejandro Hern ndez R¡os.\n", szDate, szAddr);
  printf("Web: https://www.sevenray.org/personal-identity-profile-pip-and-other-aids.html\n\n");

  PrintLongBr("Escriba su respuesta a cada pregunta y pulse <Enter>. Cuando termine la £ltima pregunta, aparecer  una ventana del navegador con los resultados. Si comete un error al responder, escriba \"deshacer\" para regresar a la pregunta anterior. Tambi‚n puede escribir \"ayuda\" para obtener una lista de otros comandos que le asistan en la revisi¢n de las respuestas anteriores.");

  fileIn = fopen(szSave, "r");
  if (fileIn != NULL) {
    Load(fileIn);
    fclose(fileIn);
LLoaded:
    if (!FFindLatest(&iPart, &i, &f)) {
      printf("AVISO: Tiene una prueba en curso. Escriba \"reiniciar\" si quiere empezar de\nnuevo.\n");
      goto LNext;
    } else
      printf("AVISO: Ha completado todas las respuestas. Escriba \"m s reciente\" para mostrar los resultados finales.\n");
  }

  for (iPart = 1; iPart <= 4; iPart++) {
LPart:
    if (iPart > 4) {
      printf("AVISO: No es posible saltar m s all  de la Parte 4.\n");
      iPart = 4;
    }
    switch (iPart) {
    case 1: PrintBanner("Parte 1/4: Preguntas iniciales");   break;
    case 2: PrintBanner("Parte 2/4: Afirmaciones");          break;
    case 3: PrintBanner("Parte 3/4: Aspiraciones m s elevadas"); break;
    case 4: PrintBanner("Parte 4/4: Rasgos a Transformar"); break;
    }
    iMax = rgPart[iPart-1];
    if (iPart == 3)
      PrintLongBr("Si bien a menudo se cree que las aspiraciones se originan en la personalidad, en realidad, son estimuladas por el poder magn‚tico del Alma que atrae a la personalidad \"hacia arriba\". A veces, una alta aspiraci¢n tiene que ver con cualidades que usted idealiza, pero que en realidad no forman parte de su sistema energ‚tico actual. En este caso, las aspiraciones son compensatorias. A veces es posible que ya no aspire a los objetivos a los que sol¡a aspirar, al menos no en la misma medida que en el pasado.");
    else if (iPart == 4)
      PrintLongBr("En el ocultismo, estos rasgos se llaman \"espejismos\" e \"ilusiones\". Se relacionan con las cualidades negativas que los individuos expresan cuando est n fuertemente influenciados por rayos particulares, pero no pueden controlar o dirigir adecuadamente la expresi¢n de estos rayos. Las energ¡as no son ni buenas ni malas; todas dependen de la condici¢n de la forma a trav‚s de la cual trabajan. Todos tenemos rasgos negativos que deber¡an transformarse en rasgos positivos. Estas expresiones negativas deber¡an correlacionarse con las energ¡as de rayos que se encuentran en su Carta de Rayos actual, o tal vez en la indicada como anteriormente, por ejemplo, en una encarnaci¢n anterior.");

    for (f = 0; f <= (int)(iPart >= 3); f++)
    for (i = 0; i < iMax; i++) {
LNext:
      iMax = rgPart[iPart-1];
      if (i < 0) {
        printf("ERROR: No es posible retroceder antes del reactivo #1.\n");
        i = 0;
      }
      if (i >= iMax) {
        printf("ERROR: No es posible avanzar m s all  del reactivo #%d.\n", iMax);
        i = iMax-1;
      }
      switch (iPart) {
      case 1: ec = GetInitResponse(i);      break;
      case 2: ec = GetStatementResponse(i); break;
      case 3: ec = GetAspireResponse(i, f); break;
      case 4: ec = GetTraitResponse(i, f);  break;
      }
      if (ec == ecNew) {
        _unlink(szSave);
        goto LNew;
      }
      if (ec == ecDone)
        goto LDone;
      if (ec == ecExit)
        goto LExit;
      if (ec == ecLast)
        i = iMax - 2;
      if (ec == ecUndo) {
        i += cmdarg;
        if (i == -1 && iPart > 1) {
          if (f <= 0) {
            iPart--;
            iMax = rgPart[iPart-1];
            f = (iPart >= 3);
          } else
            f = 0;
          i = iMax-1;
        }
        goto LNext;
      }
      if (ec == ecGoto) {
        i = cmdarg;
        goto LNext;
      }
      if (ec == ecPart) {
        iPart = cmdarg;
        goto LPart;
      }
      if (ec == ecLate) {
        if (FFindLatest(&iPart, &i, &f))
          goto LDone;
        goto LNext;
      }
      fileIn = fopen(szSave, "w");
      if (fileIn == NULL)
        printf("ERROR: ­No fue posible crear el archivo de autoguardado %s!\n", szSave);
      else {
        Save(fileIn, fFalse);
        fclose(fileIn);
      }
    }
  }
  if (!FFindLatest(&iPart, &i, &f)) {
    printf("AVISO: Ha llegado al final de la prueba, pero a£n no todos los reactivos han sido contestados\n");
    goto LNext;
  }

LDone:
  printf("AVISO: ­Ha llegado al final de la prueba. Todos los reactivos han sido contestados!\n");

  file = fopen("pip3.txt", "w");
  if (file == NULL)
    printf("ERROR: ­No fue posible crear el archivo de texto de resultados pip3.txt!\n");
  else {
    fTextFile = fTrue;
    DoResults();
    fTextFile = fFalse;
    fclose(file);
    printf("AVISO: Creado el archivo de texto de resultados pip3.txt\n");
  }

  file = fopen("pip3.htm", "w");
  if (file == NULL)
    printf("ERROR: ­No fue posible crear el archivo HTML de resultados pip3.htm!\n");
  else {
    fHTML = fTrue;
    fprintf(file, "<html><body><font face=\"Times\">\n");
    fprintf(file, "<img src=\"https://sevenray.org/wp-content/uploads/2019/08/sri-logo-nav-210h.png\">\n");
    PrintBr();
    DoResults();
    fprintf(file, "</font></body></html>\n");
    fHTML = fFalse;
    fclose(file);
    printf("AVISO: Creado el archivo HTML de resultados pip3.htm\n");
    if ((int)ShellExecute(NULL, NULL, "pip3.htm", NULL, NULL, SW_SHOW) < 32) {
      printf("ERROR: ­No fue posible inicializar el navegador HTML!\n");
      if ((int)ShellExecute(NULL, NULL, "pip3.txt", NULL, NULL, SW_SHOW) < 32)
        printf("ERROR: ­No fue posible inicializar el visor de texto!\n");
      else
        printf("AVISO: Visor inicializado para mostrar el archivo de texto de resultados pip3.txt\n");
    } else
      printf("AVISO: Navegador inicializado para mostrar el archivo HTML de resultados pip3.htm\n");
  }

  file = fopen(szSave, "w");
  if (file == NULL)
    printf("ERROR: No fue posible crear el archivo para guardar %s!\n", szSave);
  else {
    Save(file, fFalse);
    fclose(file);
    printf("AVISO: Archivo creado guardado %s\n", szSave);
  }
  file = stdout;

  pch = sz;
  sprintf(pch, "%s%%0A", szDate); while (*pch) pch++;
  for (i = 0; i < cInit; i++) {
    for (pch2 = rgResInit[i]; *pch2; pch2++) {
      if (*pch2 == '&') {
        sprintf(pch, "%s", "%26"); while (*pch) pch++;
      } else if (*pch2 == '%') {
        sprintf(pch, "%s", "%25"); while (*pch) pch++;
      } else if (*pch2 == '"') {
        sprintf(pch, "%s", "%22"); while (*pch) pch++;
      } else {
        *pch++ = *pch2;
      }
    }
    sprintf(pch, "%s", "%0A"); while (*pch) pch++;
  }
  for (i = 0; i < cStatement; i++) {
    sprintf(pch, "%d%d", rgResS[i].res, rgResS[i].resf); while (*pch) pch++;
  }
  sprintf(pch, "%s", "%0A"); while (*pch) pch++;
  for (i = 0; i < cAspire2; i++) {
    sprintf(pch, "%%20%d%%20%d", rgResA[i].res, rgResA[i].resf); while (*pch) pch++;
  }
  sprintf(pch, "%s", "%0A"); while (*pch) pch++;
  for (i = 0; i < cTrait2; i++) {
    sprintf(pch, "%%20%d%%20%d", rgResT[i].res, rgResT[i].resf); while (*pch) pch++;
  }
  sprintf(szMail, "mailto:%s?subject=PIP3%%20results%%20(­Por%%20favor%%20env¡enos%%20este%%20mensaje!)&body=%s", szAddr, sz);
  for (pch = szMail; *pch; pch++)
    *pch = (unsigned char)*pch >= 128 ? (char)rgwch437[(unsigned char)*pch - 128] : *pch;
  if ((int)ShellExecute(NULL, NULL, szMail, NULL, NULL, SW_SHOW) < 32)
    printf("ERROR: ­No fue posible inicializar su programa de correo electr¢nico!\n");
  else
    printf("AVISO: Mensaje de correo electr¢nico redactado. Por favor env¡e sus resultados a: %s\n", szAddr);

LExit:
  printf("\n");
  PrintLong("Presione <Entrar> para revisar sus respuestas desde el comienzo de la prueba. Tambi‚n puede cerrar esta ventana para salir del programa.");
  GetString(sz);
  FFindLatest(&iPart, &i, &f);
  goto LNext;
  return 0;
}
