// PIP3 - Personal Identity Profile Version 3
// Created by Michael Robbins and Rick Good
// PC program created 10/20/2018 - 4/5/2021 by Walter D. Pullen (Astara@msn.com)
// See also: https://www.sevenray.org/education/

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

// Definitions

#define szDate "April 5, 2021"
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
"full name",
"current location (city, state/province, country)",
"birth location (city, state/province, country)",
"birth time (month, day, year, minute)",
"present occupation",
"former occupations",
"education majors and favorite subjects",
"avocations or hobbies",
"major life goals",
"factors that lead you to study esotericism",
"length of time you've studied esotericism",
"length of time you've studied Alice Bailey",
"length of time you've studied the Seven Rays",
"length of time you've studied mundane astrology",
"length of time you've studied Esoteric Astrology",
"estimated Monadic Ray (1-7)",
"confidence of Monadic Ray estimation",
"estimated soul Ray (1-7)",
"confidence of soul Ray estimation",
"estimated personality Ray (1-7)",
"confidence of personality Ray estimation",
"estimated mental vehicle Ray (1-7)",
"confidence of mental Ray estimation",
"estimated astral vehicle Ray (1-7)",
"confidence of astral Ray estimation",
"estimated physical vehicle Ray (1-7)",
"confidence of physical Ray estimation",
};

const char *rgInit2[cInit] = {
"Name",
"Current location",
"Birth location",
"Birth time",
"Present occupation",
"Former occupations",
"Favorite subjects",
"Hobbies",
"Life goals",
"Factors that led to study of esotericism",
"How long studied esotericism",
"How long studied Alice Bailey",
"How long studied the Seven Rays",
"How long studied mundane astrology",
"How long studied Esoteric Astrology",
"Estimated Monadic Ray",
"Confidence of Monadic Ray estimation",
"Estimated soul Ray",
"Confidence of soul Ray estimation",
"Estimated personality Ray",
"Confidence of personality Ray estimation",
"Estimated mental vehicle Ray",
"Confidence of mental Ray estimation",
"Estimated astral vehicle Ray",
"Confidence of astral Ray estimation",
"Estimated physical vehicle Ray",
"Confidence of physical Ray estimation",
};

const char *rgInitDef[cInit] = {
"John Doe",
"Anytown, USA",
"Birth town, USA",
"Jan 1, 2000 at 12:34pm",
"Making widgets",
"Making old style widgets",
"Mathematics",
"Computers",
"Spiritual growth",
"Seeking deeper spiritual growth",
"Since 2014",
"Since 2015 when I read IHS",
"Since 2016 when I read RI",
"Since 2013",
"Since 2017 when I read EA",
"1",
"Not very confident",
"2",
"Somewhat confident",
"4",
"A balanced amount",
"5",
"It's the logical choice",
"6",
"I feel it's right",
"7",
"Very confident",
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
{3, a, norm, inc, "I am a skillful abstract thinker - one who naturally finds abstract patterns of philosophical, mathematical, theoretical or systemic relationships far more intriguing than the discovery and application of practical, concrete, artistic or even scientific knowledge - even if such specific knowledge is inherently fascinating or immediately useful."},
{6, Gb, norm, inc, "If I really love someone, I'll \"go to the ends of the earth\" to show them how I feel."},
{6, b, norm, inc, "I am known for my optimistic, enthusiastic, \"can't keep-me-down\", \"never-say-die\" attitude."},
{4, b, norm, inc, "I have lived my life soaring to the heights and plumbing the depths of human experience."},
{6, a, norm, inc, "I feel deeply that I am guided. I have complete faith in my guidance and I follow it."},
{4, a, norm, inc, "Creating beauty in everything I do is absolutely indispensable to me; I seek to become the \"artist of my life\"."},
{7, G, norm, inc, "I'm one for \"straightening things up\", and \"sorting things out\". \"A place for everything and everything in its place\" is my motto."},
{1, a, norm, inc, "Whatever life may throw at me, I stand in strength - firm, steadfast, unafraid - persisting until the end."},
{1, b, norm, inc, "I destroy the old and unnecessary with much less hesitation than most other people."},
{2, G, norm, inc, "I almost always <i>identify</i> (immediately and deeply) with people's feelings and states of mind - absorbing them as if they were my own."},
{1, a, norm, inc, "I am a natural leader."},
{3, a, norm, inc, "Above all I am a <i>thinker</i>, using my intellect to think in the <i>broadest terms</i> about whatever subject I am considering."},
{1, a, norm, inc, "Compared to others, I have an extremely strong and steadfast will."},
{7, b, norm, inc, "I am an \"organization-person\" - actually, a 're-organization person' - one whose predominant strength lies in designing, building, or efficiently managing all the many details of the newer kind of organizations (large or small) needed to help humanity solve its many problems - whether legal, financial, social, religious or governmental, etc."},
{5, G, norm, inc, "I am particularly good at applied mathematics - the kind of mathematics that calls for accurate calculations and for skill in various arithmetic operations that can be applied to the solution of practical problems."},
{1, G, norm, inc, "When I compare myself to others, I can honestly say that I am an unusually powerful person - strength and steadfastness are two of my <i>commanding</i> virtues."},
{4, b, norm, inc, "I fight, then make-up - fight again, then make-up again. It seems I can't help but do both!"},
{4, b, norm, inc, "I love being dramatic, and my life is filled with dramatic experiences and emotional contrasts."},
{2, b, norm, inc, "Give me <i>wisdom</i> above all; I value it more than many very essential qualities - power, love, intelligence, beauty, knowledge, idealism, order, etc. Wisdom comes first."},
{1, G, norm, inc, "I am one who directs others."},
{4, b, norm, inc, "More than most people, my life has been filled with conflict (inner, outer, or both); very often I have felt torn between conflicting forces."},
{4, b, norm, inc, "My usual approach to doing things is characterized by bursts of incessant activity, followed by nearly equal periods of complete lethargy during which (even though I feel I should be active) I just can't get myself to do a thing. Then, suddenly, my mood changes, and again I fly into action."},
{5, G, norm, inc, "I frequently approach problems or tasks through quantification. I analyze or evaluate a problem or task in terms of numbers, and determine a solution accordingly."},
{2, b, norm, inc, "All paths to the center of being-whether the paths of power, love, intelligence, beauty, knowledge, ecstasy, magic, etc.-are of equal value, but my particular path is the eternally patient pursuit of pure heart-felt wisdom, complete enlightenment and absolute truth."},
{4, a, norm, inc, "I simply love color and have a highly developed color sense that I express, in one way or another, at every opportunity."},
{6, a, norm, inc, "When I consider all the various ways of raising humanity, I think faith and belief in God, in some Higher Power or in some great Source of Truth are the most important."},
{1, b, norm, inc, "I am prepared, equipped and inclined to force matters and issue ultimatums whenever I think it necessary."},
{5, a, norm, inc, "I place a very high value upon strict, factual accuracy-even when others might lose patience with such precision and exactitude."},
{7, a, norm, inc, "I am almost always extremely careful about handling even the smallest details; I want things done properly."},
{2, G, norm, inc, "I always seek to develop a complete understanding of people-a deep, intuitive, 'heart to heart' understanding."},
{5, a, norm, inc, "Whenever I conduct a study, I do so rigorously and systematically-thoroughly mastering all the factual detail of my subject."},
{1, Gb, norm, inc, "I am well endowed with the dynamic power to smash through obstacles-and I do!"},
{5, b, norm, inc, "I am actively involved (vocationally or avocationally) in one or more of the \"exact sciences\" such as physics, chemistry, astronomy, biology, electronics, computer science, etc. or their many combinations extensions or applications."},
{5, b, norm, inc, "I approach problem solving through first-hand observation, carefully examining all the particulars. Then I begin experimenting methodically, using a \"trial and error\" procedure, and I persist until I've proven to myself that I have a solution that really works."},
{1, b, norm, inc, "I am one who \"takes the Kingdom of Heaven by storm\"."},
{2, G, norm, inc, "I have a highly developed ability to perceive clearly and with intuitive foresight. This ability is especially keen when applied to understanding people-their potentials, motives, problems and relationships."},
{2, a, norm, inc, "I have a deep-seated need to nurture and comfort people, even if I don't know them personally."},
{7, a, norm, inc, "I'm a habitual time-scheduler and list-maker; that's how I make sure I do everything I have to do, at the time it should be done, and in the time I have to do it."},
{3, a, norm, inc, "Though there are many worthy pursuits, I live my life principally as an intellectual, mostly engaged in the world of thought."},
{1, b, norm, inc, "I have no real difficulty terminating or discarding things. In fact, it often exhilarates me-especially when something old and outworn has to be eliminated so that something fresh and new can take its place."},
{5, b, norm, inc, "I enjoy tinkering, and taking things apart (pieces of equipment, mechanical devices, various objects, etc.,) to see how they are put together, how they work, and how they can be repaired."},
{6, G, norm, inc, "I often vow to God (or to myself or to others) that I will uphold some high ideal."},
{6, a, norm, inc, "I long to live my life with faithfulness and loyalty, earnestly serving those I love."},
{3, G, norm, inc, "I talk a great deal-more than most people. My mind is usually filled with many interesting thoughts, and I just have to give them verbal expression."},
{1, G, norm, inc, "I have a striking ability to initiate action, galvanizing others into activity. I deliver the hammer stroke that sets things vibrating."},
{3, a, norm, inc, "My writing style is often abstract, complex and rather involved. I enjoy elaboration and ramification, and I extend my mind into numerous fields in order to combine many strands of thought."},
{4, a, norm, inc, "Experiencing the beauty that arises from the harmonious, aesthetically appealing arrangement of my personal and social surroundings is vital to my happiness."},
{7, b, norm, inc, "My task is to \"bring Heaven to Earth\"-not just in general as all servers do, but through the <i>excellent expression of archetypal heavenly forms</i> through <i>perfected earthly forms.</i>"},
{3, G, norm, inc, "I enjoy \"figuring the odds\"-speculating and conjecturing on the probabilities."},
{7, a, norm, inc, "Leave the practical details to me; I will organize them and execute them in the most efficient way."},
{3, G, norm, inc, "I reason with unusual facility about highly abstract, theoretical issues in which a complex web of abstract relationships must be clearly and exactly understood. (For example: higher mathematics, academic philosophy, economic theory, commodity and stock projections, historical trends, etc.)"},
{7, a, norm, inc, "In almost everything I do, the most natural thing is for me to develop a routine or personal ritual-a regular pattern of action which works efficiently for me, and which I like to repeat."},
{4, G, norm, inc, "I compromise too readily when; instead, I should stand up for principles."},
{1, a, norm, inc, "I am a resolute individual, highly courageous in the face of all kinds of dangers."},
{3, b, norm, inc, "When I consider all the various ways of raising humanity, I think good business, intelligent economics; sound monetary practices and widespread financial opportunity are the most important."},
{5, G, norm, inc, "I like to read scholarly or technical publications that explain, in considerable detail, developments in certain specialized fields of enquiry."},
{1, G, norm, inc, "When opposed by anyone or anything, my <i>will to conquer</i> rises immediately."},
{5, G, norm, inc, "I am animated by the spirit of scientific discovery, and derive enjoyment and satisfaction from the pursuit of scientific investigation."},
{1, a, norm, inc, "I am firmly purposeful in almost everything I think or do."},
{4, G, norm, inc, "Not only am I irresistibly drawn to the creative arts, but also I express and release my emotions through some form of artistic creativity."},
{5, G, norm, inc, "I generally have a very objective attitude, arising from a cool, dispassionate, highly mental and analytical examination of life situations."},
{1, a, norm, inc, "I have the strength and independence to stand alone; I require the support and company of others far less than most people do."},
{2, G, norm, inc, "Because it does my heart good to see people grow and develop, one of my greatest pleasures is to be a teacher."},
{6, a, norm, inc, "I hold those I love very close to me; I feel utterly devoted to them and inseparably connected."},
{7, b, norm, inc, "I am a practical ritualist-one who uses organized movement, regulated thought, and the rhythmic word (as well as the magical language of color, music and symbol) to invoke the higher forces to humanity's aid."},
{2, a, norm, inc, "In my own quiet, gentle and loving way, I make it a point to emphasize the positive. I almost always have something good to think or say about people."},
{2, a, norm, inc, "I draw people together through warmth and magnetism, and my love helps to hold them together. "},
{1, G, norm, inc, "For whatever reason, I am strongly inclined to be dominant. I do not shrink from imposing authority."},
{5, Ga, norm, inc, "My intellect is precise, highly analytical, and sharply focused on factual knowledge and specific, concrete issues."},
{5, a, norm, inc, "I give close, detailed (yet intuitive) study to even the broadest of subjects; some call my approach pedantic, but my aim is to achieve complete comprehension of all related factors, becoming so thoroughly familiar with a subject that (using my preferred method of \"meticulous entirety\") I absorb that subject entirely."},
{6, b, norm, inc, "When I pursue a high ideal, I devote myself to it one-pointedly with fervent emotional intensity."},
{6, G, norm, inc, "I hold fast to certain lofty ideals that I frequently and enthusiastically reaffirm to myself and emphasize to others. These ideals are my \"guiding star\", and by them I chart my course in life."},
{7, G, norm, inc, "I think appearances are very important; I invest considerable energy promoting good appearance in all aspects of my life and circumstances."},
{6, b, norm, inc, "Whenever I \"go for\" something, I go for it passionately, with unbounded enthusiasm."},
{3, b, norm, inc, "My agile, intelligent mind is well suited to work in the field of economics and finance."},
{5, b, norm, inc, "When technical problems arise, I'm in my element as the \"trouble-shooter\"."},
{6, a, norm, inc, "I pray in complete trust that my prayers are always answered by God or some Higher Power, and in humble willingness to accept whatever answers or responses I receive."},
{4, a, norm, inc, "I love to create colorful and aesthetically appealing ensembles, compositions or mixtures by imaginatively making an artistic unity of things that are not usually found together."},
{4, b, norm, inc, "The feeling of being \"down\" and discouraged comes over me frequently, but the mood soon lifts and I'm \"up\" and ready to try again."},
{3, b, norm, inc, "I enjoy moving money here and there according to my best anticipation of profit."},
{3, b, norm, inc, "My intelligently adaptable nature and my shrewd foresight of commercial trends make business activity my natural field of expression."},
{3, b, norm, inc, "More than many people, I like to \"be smart\" with money-speculating, investing and using my wits to generate profit."},
{4, G, norm, inc, "I think, write, and speak in colorful words and images."},
{1, b, norm, inc, "When I am in a personal or professional situation that should not continue as it is, I prefer drastic and liberating adjustments that end the old conditions abruptly."},
{3, b, norm, inc, "I especially enjoy those aspects of any business enterprise that call for me to arrange transactions cleverly and creatively."},
{5, a, norm, inc, "When I explain something, I make certain I am extremely lucid, logical and specific (offering concrete examples to illustrate my point), so my listener can grasp every single detail with complete clarity."},
{2, G, norm, inc, "I continually meditate and study for the purpose of achieving complete enlightenment and profound understand of truth-even though such time-consuming, exhaustive understanding may not have an immediate, practical application in present circumstances."},
{4, G, norm, inc, "I love color; but I don't just love it-I <i>produce it</i> in all I create, and I <i>express my color-sense</i> in the clothing that I wear."},
{7, a, norm, inc, "I am one of those who requires that all things to be done decently and in order."},
{5, b, norm, inc, "When I look at a piece of equipment, my first thought is \"how does it work?\" then I set myself to finding out."},
{3, G, norm, inc, "I spend a great deal of time communicating a broad range of ideas and information through whatever means are available to me-whether word-of-mouth or the many types of media."},
{6, G, norm, inc, "I am marching towards the Light."},
{6, G, norm, inc, "I frequently experience strong feelings of devotion."},
{2, b, norm, inc, "I love and deeply respect the Christ and His example of pure love, but my nature inclines me to seek enlightenment and Self-realization through the application of my heart and meditative mind to the profound wisdom taught by the Buddha."},
{7, G, norm, inc, "I focus a good deal of attention on the body-on rules and regimens to keep it vitally healthy, or on programs for cultivating its full potential."},
{3, G, norm, inc, "I'm very good at \"plotting and planning\" - figuring out what I will do many moves ahead. I'm really an excellent strategist."},
{1, G, norm, SKIP, "Compared to others I am a powerful individual - strength and steadfastness are two of my predominant virtues."},
{4, Ga, norm, SKIP, "Not only am I irresistibly drawn to the creative arts, but also I express and release my emotions through some form of artistic creativity."},
{5, b, norm, inc, "I feel at home in laboratories, and would enjoy working in some kind of laboratory setting, where knowledge is pursued and confirmed experimentally."},
{6, G, norm, inc, "I desire to give myself totally to a worthy or an ideal cause."},
{7, a, norm, inc, "I am one who really understands the value and importance of upholding the law and \"playing by the rules\"."},
{2, b, norm, inc, "Through profound study and extended deep meditation, I am determined to achieve an intuitive, totally inclusive yet meticulously detailed understanding of the patterned beauty of the whole."},
{6, a, norm, inc, "I love to worship God, and am filled with reverence for great saints and sages who exemplify the Creator's Ways."},
{3, b, norm, inc, "I am a tactician and strategist-a person who always likes to have a \"game plan\"."},
{6, b, norm, inc, "I am a modern 'crusader' in a noble cause."},
{2, a, norm, inc, "My way is to freely share the radiant sunshine of heart-felt love with as many people as possible."},
{1, Gb, norm, inc, "I tell the truth in a straight and direct manner; it may hurt or disturb some, but the truth is the truth and I am not afraid to tell it just as it is."},
{3, G, norm, inc, "The unusual flexibility and fertility of my mind make it easy for me to modify and adapt ideas in all kinds of ways; I'm very good at helping people see things from a variety of novel angles, thereby multiplying their options."},
{7, G, REVERSE, inc, "Some people say my \"head is in the clouds\", but I do not worry myself or others over what I consider little unimportant things. Though to others such things are so often important, to me they are usually just trifles that I just can't be bothered with."},
{4, a, norm, inc, "I'm always finding myself \"in the middle\"- a mediating bridge and an agent of reconciliation between contradictory opinions and contending forces."},
{5, a, norm, inc, "Not only do I enjoy doing research, but I also have the patience and perseverance to track the smallest fact to its source in order to ensure the accuracy of my research."},
{4, a, norm, inc, "I am a skillful \"peace-maker\" because I almost always have a true understanding of \"both sides\" of conflicted situations and actually enjoy the often stressful give-and-take process of finding those points of mutual agreement that are necessary for resolving conflict into harmony."},
{2, a, norm, inc, "It is easier for me than it is for most to establish kind, warm and loving relationships with the people I meet."},
{2, G, norm, inc, "It is most natural for me to approach any important goal patiently and sensitively. I accomplish much through tact, considerateness, and the wise use of slow action."},
{7, G, norm, inc, "I attach real importance to the manner and style in which a thing is done. I'm a great one for doing everything \"in good form\"."},
{2, b, norm, inc, "Though there are many ways to express love, my particular way is to blend love with austere wisdom rather than simply to pour forth pure, unconditional love."},
{5, a, norm, inc, "Being intent on distinguishing one thing from another, I seek to discover or learn the correct classification or name for anything I examine, so that I can know exactly what a thing is and what it isn't."},
{3, G, norm, inc, "I'm a <i>very</i> active individual-many have said, \"hyper-active\". There are so many things to do and one just can't get them done by moving slowly."},
{7, a, norm, inc, "When I arrange things, I am more particular than most people about making the arrangement exact, accurate or \"just so\"!"},
{7, b, norm, inc, "I am constantly restructuring the existing order of things, renovating and re-designing the practical aspects of daily living-whether in my own personal and professional sphere, or in a much larger context."},
{1, a, norm, inc, "I am usually the power at the center of things; it is a position that is completely natural for me."},
{6, b, norm, inc, "When I truly aspire to reach a goal, I \"catch fire\", and \"burn with enthusiasm\" until I get there."},
{3, a, norm, inc, "I operate on the philosophical premise that truth can actually be reached without scientific experimentation, principally by using rigorous, logical, deductive thinking-provided that one's premises are correct and one's reasoning is sound."},
{4, a, norm, inc, "Because disharmony always makes me feel extremely uncomfortable, I act immediately to restore harmony as quickly and as skillfully as possible."},
{3, a, norm, inc, "I am a philosopher-an intellectual. In my search for broad philosophical comprehension, the intricate, endlessly complex world of the intellect is my true home."},
{1, G, norm, inc, "I have a masterful way of directing people to do my will or the Will of a Greater Being-in fact I insist they do so."},
{2, a, norm, inc, "Often I feel an overflowing of sympathy and compassion for all people. At such special times, I love and forgive all unconditionally, no matter who they are, or what they may have done."},
{7, b, norm, inc, "I have a very real ability to bring rhythmic order out of the chaos of human living, by 'grounding' transformational ideas into practical activity, and \"seeing them through\" to perfected expression."},
{7, b, norm, inc, "I am a modern 'magician'. Being to a sufficient extent familiar with the angelic and elemental forces of nature, I carefully and capably utilize the practical laws of will and thought to link spirit with matter for the benefit of humanity."},
{1, G, norm, inc, "When I determine what must be done, I absolutely refuse to let sentiment, emotion or attachments to people prevent me from fully executing my intention."},
{4, b, norm, inc, "I am both courageous and cowardly (and often at the same time); which of the two I express depends on my mood, which often changes."},
{4, G, norm, inc, "Whenever tension builds up, I almost always say or do something humorous to release the tension."},
{7, a, norm, inc, "I derive great satisfaction from devising or participating in thoroughly planned, highly structured programs of action, in which appropriate procedures are clearly detailed, and intended results are clearly defined."},
{3, a, norm, inc, "I delight in mental gymnastics and the demonstration of intellectual virtuosity."},
{4, G, norm, inc, "My usual way of writing and speaking is to express my thoughts with feeling-freely, spontaneously, and often poetically."},
{4, G, norm, inc, "I'm very good at entertaining and amusing people with sparkling, imaginative conversation, but I'm just as likely to reverse myself become introspectively or even gloomily silent."},
{5, b, norm, inc, "I not only have deep interest in inventing, but some ability as an inventor in the field of technology."},
{2, a, norm, inc, "I constantly practice loving kindness in all my life encounters and experiences."},
{6, G, norm, inc, "Whenever my cherished ideals are challenged, I zealously rise to their defense."},
{6, a, norm, inc, "I often long to be where everything is pure and perfect-call it \"utopia\" or call it \"paradise\"."},
{7, b, norm, inc, "The way I live my life can accurately be described as \"ritualistic\"; ceremony, magic and resultant manifestation are major themes in my consciousness and activity."},
};

const TRAIT rgAspire[cAspire] = {
{1, "I aspire to become a dynamic, powerful and benevolent leader - strong, firm and steadfast in my service to the Highest Law and my defense of the Greatest Good."},
{2, "I aspire to a deep, intuitive, loving-understanding of people so that I can nurture them, and help them unfold their highest potential."},
{3, "I aspire to a resourceful and versatile mentality so that I can apply it creatively to numerous intellectually challenging enterprises."},
{4, "I aspire to express great artistry in every aspect of my life, thereby inspiring others to greater creativity and a greater love of beauty."},
{5, "I aspire to become vitally involved in discovering the keys and solutions to Nature's mysteries through advanced scientific research and experimentation."},
{6, "I aspire to live with ardent faith and passionate commitment, unshakably loyal to my system of values and true to my highest ideals - my \"Vision of the Right.\""},
{7, "I aspire to bring a beautiful rhythmic order out of the chaos of human living, by \"grounding\" transformational ideas in practical activity, and \"seeing them through\" to perfected expression."},
{1, "I aspire to shatter chains, liberating people from all forms of bondage and oppression."},
{2, "I aspire to a wise and intuitive, deep and complete understanding of life so that I can teach and illumine others."},
{3, "I aspire to create a thoroughly comprehensive, totally rational, philosophical theory of truth, through the power of deep thinking and the most careful reasoning."},
{4, "I aspire to be a mediator, a harmonizer, a \"bridge of at-one-ment\" - a reconciler of human conflicts, and an instrument of peace."},
{5, "I aspire to invent or develop those mechanisms, instruments, or technical improvements which will transform and uplift the quality of human life."},
{6, "I aspire with complete faith and utter devotion to serve \"God,\" the worthiest cause, or the \"highest ideal\" I can conceive."},
{7, "I aspire to involve myself deeply in the art and science of creative, organizational transformation, redesigning and re-structuring the patterns of human interaction, so that \"a better world\" may more rapidly emerge."},
};

const TRAIT rgTrait[cTrait] = {
{7, "Over-concern for rules and regulations."},
{6, "Uncontrolled, misguided enthusiasm."},
{5, "Too much skepticism."},
{4, "Too many \"highs\" and \"lows.\""},
{7, "Rigidity and inhibited self-expression."},
{6, "Excessive emotional intensity."},
{7, "Subservience to habit and routine."},
{5, "\"Can't see the forest for the trees.\""},
{3, "Too much talking and hyper-active thinking that does not lead to tangible achievement."},
{2, "The love of being loved."},
{1, "Excessive egoism, self-centeredness and self-importance; too determined to be \"the one at the center.\""},
{1, "Selfish ambition for power."},
{2, "Too much study without taking action."},
{3, "Intellectual pride."},
{4, "A tendency to self-contradiction, inconsistency and unpredictability."},
{5, "Too much intellectual analysis and dissection."},
{6, "Over-leaning on others."},
{7, "Over-concern for orderliness and detail."},
{4, "Over-eagerness for compromise; \"peace at any price.\""},
{6, "Over-eagerness to trust and believe; misplaced devotion."},
{3, "Wasted energy through over-activity, scattered living and restlessness."},
{3, "Manipulativeness, deviousness and active scheming."},
{2, "Fear because of over-sensitivity."},
{1, "Desire to dominate and control others."},
{1, "Willfulness."},
{2, "Giving-in too easily."},
{5, "Intolerance of ideas which do not fit in with established knowledge."},
{4, "Debilitating struggle, trouble and turmoil."},
};

const char *rgszResponse[cResponse+1] = {"Not Answered", "Definitely True", "Usually True", "More True Than False", "In Between - As True as False", "More False Than True", "Usually False", "Definitely False"};
const char *rgszResponseAbbrev[cResponse+1] = {"NA", "DT", "UT", "MT", "IB", "MF", "UF", "DF"};

const char *rgszColor[cRay+3] = {"Black", "Red", "#191970", "#008000", "#c0c000", "#ffa500", "#ff3edf", "#9900CC", "White", "Yellow"};

const int rgPart[cPart] = {cInit, cStatement, cAspire2, cTrait2};


// Variables

char rgResInit[cInit][cchSzMax];
RESPONSE rgResS[cStatement];
RESPONSE rgResA[cAspire2];
RESPONSE rgResT[cTrait2];
flag fInfo = fFalse, fHTML = fFalse;
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

  for (i = 0; i < cchSzMax*2 && !feof(fileIn) && (ch = getc(fileIn)) >= ' '; i++)
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
      fprintf(file, " ");
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
        printf("  ");
      printf("  %s\n", pch1);
      pch1 = pch2 + 1;
      fLater = fTrue;
    } 
  }
  if (fExtra && fLater)
    printf("  ");
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
    fprintf(file, "<b>%s</b>", sz);
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
    printf("ERROR: Save file variable %d below range from %d to %d! Assuming %d.\n", *n, n1, n2, n1);
    *n = n1;
  }
  if (*n > n2) {
    printf("ERROR: Save file variable %d above range from %d to %d! Assuming %d.\n", *n, n1, n2, n2);
    *n = n2;
  }
}

void Load(FILE *fileIn)
{
  char sz[cchSzMax], ch;
  int i, n;

  ReadLine(fileIn, sz);
  if (!FEqSz(sz, szDate))
    printf("WARNING: Save file date (%s) not equal program date (%s).\n", sz, szDate);
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
  if (FEqSz(sz, "help")) {
    printf("\n");
    PrintLong("Welcome to PIPiii help. Type your answer to each question and then press <Enter>. You can also type any of the following commands:");
    printf("  undo: If you make a mistake answering, type \"undo\" to back up one question.\n");
    printf("  back <#>: For example, \"back 7\" to back up 7 questions.\n");
    printf("  next <#>: For example, \"next 3\" to skip the next 3 questions.\n");
    printf("  goto <#>: For example, \"goto 109\" to jump to question #109.\n");
    printf("  part <#>: For example, \"part 2\" to jump to Part 2: Statements section.\n");
    printf("  latest: Jump to first unanswered question, or display results if done.\n");
    printf("  restart: Restart the test from scratch, erasing all previous responses.\n");
    printf("  help: Display this help list.\n\n");
    return ecRetry;
  }
  if (FPrefixSz(sz, "undo") || FPrefixSz(sz, "back")) {
    cmdarg = -atoi(sz + 4);
    if (cmdarg >= -1) {
      cmdarg = -1;
      printf("NOTICE: Backing up one question.\n\n");
    } else
      printf("NOTICE: Backing up %d questions.\n\n", -cmdarg);
    return ecUndo;
  }
  if (FPrefixSz(sz, "next")) {
    cmdarg = atoi(sz + 4);
    if (cmdarg <= 1) {
      cmdarg = 1;
      printf("NOTICE: Jumping forward one question.\n\n");
    } else
      printf("NOTICE: Jumping forward %d questions.\n\n", cmdarg);
    return ecUndo;
  }
  if (FPrefixSz(sz, "goto")) {
    cmdarg = atoi(sz + 4);
    if (cmdarg <= 1)
      cmdarg = 1;
    printf("NOTICE: Jumping to question #%d.\n\n", cmdarg);
    cmdarg--;
    return ecGoto;
  }
  if (FPrefixSz(sz, "part")) {
    cmdarg = atoi(sz + 4);
    if (cmdarg <= 1)
      cmdarg = 1;
    printf("NOTICE: Jumping to Part %d.\n\n", cmdarg);
    return ecPart;
  }
  if (FEqSz(sz, "latest")) {
    printf("NOTICE: Jumping to latest answered question.\n\n");
    return ecLate;
  }
  if (FEqSz(sz, "restart")) {
    printf("NOTICE: Restarting test from scratch!\n\n");
    return ecNew;
  }
  if (ch == 'i') {
    fInfo = !fInfo;
    printf("NOTICE: Info mode %s.\n", fInfo ? "ON" : "OFF");
    return ecRetry;
  }
  if (ch == 'z') {
    printf("NOTICE: Jumping to last question in current section.\n\n");
    return ecLast;
  }
  if (ch == 'f') {
    printf("NOTICE: Displaying results now.\n\n");
    return ecDone;
  }
  if (ch == 'x') {
    printf("NOTICE: Exiting program now.\n");
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
      printf("NOTICE: All answers randomized. Enter \"latest\" to display results now.\n");
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
      printf("NOTICE: All answers set to choice #%d. Enter \"latest\" to display results now.\n", n);
    }
    return ecRetry;
  }
  return ecNormal;
}

int GetInitResponse(int i)
{
  char sz[cchSzMax];
  int ec;

  printf("Initial question #%d/%d:\n", i+1, cInit);
  if (rgResInit[i][0])
    printf("NOTICE: You've answered this question before. You said: %s\n", rgResInit[i]);
LRetry:
  printf("Enter your %s: ", rgInit[i]);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && !rgResInit[i][0]) {
    printf("ERROR: Please enter something (even if \"anonymous\" or \"unknown\").\n");
    goto LRetry;
  }
  if (sz[0])
    sprintf(rgResInit[i], "%s", sz);
  else
    printf("NOTICE: Keeping old answer for this question: %s\n", rgResInit[i]);
  Print("\n");
  return ec;
}

int GetStatementResponse(int s)
{
  char sz[cchSzMax];
  const STATEMENT *ps;
  int i, f, n, n1, n2, nPrev, ec;

  ps = &rgStatement[s];
  printf("Statement #%d/%d:\n", s+1, cStatement);
  sprintf(sz, "%s", ps->sz);
  PrintIndent(sz, fFalse);
  if (fInfo)
    printf("INFO: Ray %d%s%s, %s, %s\n", ps->ray, (ps->nType & a) > 0 ? "a" : ((ps->nType & b) > 0 ? "b" : ""), (ps->nType & G) > 0 ? " general" : "", ps->fReverse ? "REVERSED" : "normal", ps->fSkip ? "SKIP" : "include");
  printf("The statement above can be:\n");
  for (i = 1; i <= cResponse; i++)
    printf("  %d) %s: %s.\n", i, rgszResponseAbbrev[i], rgszResponse[i]);
  for (f = 0; f <= 1; f++) {
    nPrev = !f ? rgResS[s].res : rgResS[s].resf;
    if (nPrev)
      printf("NOTICE: You've answered this statement before. You said: %d (%s)\n", nPrev, rgszResponseAbbrev[nPrev]);
LRetry:
    printf("Enter how the statement applies to you %s (1-%d): ", f == 0 ? "NOW" : "in the past FORMERLY", cResponse);
    ec = GetString(sz);
    if (ec == ecRetry)
      goto LRetry;
    else if (ec > ecNormal)
      return ec;
    if (!sz[0] && nPrev != 0) {
      printf("NOTICE: Keeping old answer for this statement: #%d (%s)\n", nPrev, rgszResponseAbbrev[nPrev]);
      goto LDone1;
    }
    n = atoi(sz);
    n1 = n / 10; n2 = n % 10;
    if (f < 1 && n1 >= 1 && n1 <= cResponse && n2 >= 1 && n2 <= cResponse) {
      rgResS[s].res = n1;
      rgResS[s].resf = n2;
      printf("NOTICE: Assigning response %d (%s) to NOW and response %d (%s) to FORMERLY.\n", n1, rgszResponseAbbrev[n1], n2, rgszResponseAbbrev[n2]);
      goto LDone2;
    }
    if (n < 1 || n > cResponse) {
      printf("ERROR: Bad response. Please enter a number from 1 to %d.\n", cResponse);
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

  printf("Highest Aspirations #%d/%d:\n", t+1, cAspire-1);
  if (t > 0) {
    printf(" Aspirations you've already ranked:\n");
    for (i = 0; i < t; i++) {
      n = !fFormer ? rgResA[i].res : rgResA[i].resf;
      if (n < 1)
        continue;
      pt = &rgAspire[n-1];
      sprintf(sz, "#%d (%d): %s", i+1, n, pt->sz);
      PrintIndent(sz, fTrue);
    }
    printf("\n Aspirations you have NOT ranked yet:\n");
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
    printf("NOTICE: You've answered this aspiration before. You selected: #%d\n", nPrev);
LRetry:
  printf("For you %s, which aspiration above most applies (1-%d): ", !fFormer ? "NOW" : "in the past FORMERLY", cAspire);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && nPrev != 0) {
    printf("NOTICE: Keeping old answer for this aspiration: #%d\n", nPrev);
    goto LDone;
  }
  n = atoi(sz);
  if (n < 1 || n > cAspire) {
    printf("ERROR: Bad aspiration. Please enter a number from 1 to %d.\n", cAspire);
    goto LRetry;
  }
  if (FResponsedAlready(rgResA, t, n, fFormer)) {
    printf("ERROR: Aspiration %d already selected. Please enter a new number.\n", n);
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

  printf("Traits to Transform #%d/%d:\n", t+1, cTrait2);
  if (t > 0) {
    printf(" Traits you've already ranked:\n");
    for (i = 0; i < t; i++) {
      n = !fFormer ? rgResT[i].res : rgResT[i].resf;
      if (n < 1)
        continue;
      pt = &rgTrait[n-1];
      sprintf(sz, "#%d (%d): %s", i+1, n, pt->sz);
      PrintIndent(sz, fTrue);
    }
    printf("\n Traits you have NOT ranked yet:\n");
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
    printf("NOTICE: You've answered this trait before. You selected: #%d\n", nPrev);
LRetry:
  printf("For you %s, which trait above most applies (1-%d): ", !fFormer ? "NOW" : "in the past FORMERLY", cTrait);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && nPrev != 0) {
    printf("NOTICE: Keeping old answer for this trait: #%d\n", nPrev);
    goto LDone;
  }
  n = atoi(sz);
  if (n < 1 || n > cTrait) {
    printf("ERROR: Bad trait. Please enter a number from 1 to %d.\n", cTrait);
    goto LRetry;
  }
  if (FResponsedAlready(rgResT, t, n, fFormer)) {
    printf("ERROR: Trait %d already selected. Please enter a new number.\n", n);
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

  PrintBanner("PIP III RESULTS");
  PrintLine("PIP III: The Personal Identity Profile Version 3.");
  PrintLine("Copyright (c) 2010-2021 by Michael Robbins and Rick Good. All Rights Reserved.");
  sprintf(sz, "PC program release: %s", szDate);
  PrintLine(sz);
  if (!fHTML)
    PrintLong("Web: https://www.sevenray.org/education/");
  else
    fprintf(file, "For more information visit: <a href=\"https://www.sevenray.org/education/\">https://www.sevenray.org/education/</a><br>\n");
  PrintBr();

  PrintLongBr("The PERSONAL IDENTITY PROFILE Version III is designed to describe your personality and Transpersonal Self or Soul in terms of certain essential subtle energies that condition all life on earth. According to the teachings of the newest form of psychology - esoteric psychology - these foundational energies emanate from the Sun, and planets of our solar system, from the twelve constellations of the zodiac, and from three major constellations (Sirius, the Pleiades and the Great Bear). From the perspective of esoteric psychology, the energies with which advanced humanity must immediately learn to work are energies emanating from the seven major stars in the Great Bear. The Ageless Wisdom Teaching asserts that the heavenly bodies are far more than massive, inanimate sky-objects; on the contrary, according to the esoteric sciences, the planets, stars and constellations are thought to be the \"physical bodies\" of great celestial Entities Who have immense intelligence and influence upon all human beings, and many other forms of life of greater and lesser magnitude.");
  PrintLongBr("While there are influential Entities within the local cosmo-system (namely, many undiscovered planets and planetoids within our solar system, and many neighboring stars which all have their contributing \"astro-chemical\" influences), the present form of the PERSONAL IDENTITY PROFILE III deals only with the energy influences of the most important of these Entities: the seven major stars (Entities) of the Great Bear, from which emanate the seven energies known as the Seven Rays.");

  PrintBanner("DEMOGRAPHIC INFORMATION");
  for (i = 0; i < cInit; i++) {
    sprintf(sz, "%s: %s", rgInit2[i], rgResInit[i]);
    PrintLine(sz);
  }
  PrintBr();

  for (nWhat = 0; nWhat < 3; nWhat++) {
    switch (nWhat) {
    case 0: pch = "RAYS"; break;
    case 1: pch = "RAY TYPES"; break;
    case 2: pch = "RAY LINES"; break;
    }
    PrintBanner(pch);

  if (nWhat == 0) {
  } else if (nWhat == 1) {
    PrintLongBr("Although there are probably `seven` types related to each of the Seven Rays, PIP III emphasizes only two major types which correlate with hints which the Tibetan Teacher has given in various of His books. In some cases the distinction between TYPE A and TYPE B relates to the degree of `abstraction` or the degree of `concretion` as in the cases of Types 3A and 3B, and Types 5A and 5B, but not in all type distinctions. For practical purposes, we might think of the two Ray-Types offered as two types which `frequently appear` in relation to a particular ray.");
    PrintLongBr("It should be understood that it is possible for you to score high on one `type` of a given ray and low on the other `type`. So often, people on the same ray may not recognize each other because they are examples of different types. Master DK, for instance, has given three types in connection with the second ray (especially the second ray soul). Those on the Wisdom or Love aspects of the second ray may not recognize each other immediately as belonging to the `same ray`.");
    PrintLongBr("Some people using the PIP have found the RAY-TYPES CHARTS even more indicative of their rays (i.e., their Ray Formula) than the RAYS CHARTS (considering the rays as seven great divisions of energy). One thing is for certain: if you score very high on one of the types of a ray (even if not on the other type) that ray is probably strong in your energy system. The only exception might be a high score on R1A which seems to evoke a high score even from those who do `not` have a strong R1 - but `~wish~` they had.");
    PrintLong("This chart has fourteen bar-scales, representing two major types for each ray. For a truly detailed description of the types, please study `Tapestry of the Gods` - Vols. I and II. The type divisions are as follows:");
    StartList();
    PrintList(1, "~R1A:~ The Strong and Steadfastness Type of the first ray, much correlated with the influence of Vulcan and Saturn. This scale generally tends to be `too elevated` in the case of many people. When authentic, it indicates the ability to `stand steady` despite the onslaught of opposing forces.");
    PrintList(1, "~R1B:~ The Destructive Type of the first ray, much correlated with the influence of Pluto and perhaps Mars. It is rare that those who choose R1A for reasons of \"social desirability\" (or 'esoteric desirability') choose R1B for similar reasons. When R1B is high it almost certainly indicates the presence of the first ray in the energy system and perhaps the first ray soul.");
    PrintList(2, "~R2A:~ The Pure Love Type of R2. Those who strongly express the Christ Aspect of the second ray score high on this Ray-Type. This type may at first be somewhat lacking in those whose approach to spirituality involves, strictly speaking, the pursuit of Wisdom.");
    PrintList(2, "~R2B:~ The Wisdom Type of R2. Those who strongly express the Buddha Aspect of the second ray score high on this Ray-Type. This type may be somewhat lacking in the expression of love, emphasizing wisdom instead, though in our solar system, Love and Wisdom can hardly be separated from each other.");
    PrintList(3, "~R3A:~ The Abstract Thinking Type of R3: This type tends to be elevated in those who like to think abstractly in relation to philosophical, mathematical, economic or, in general, speculative subjects. In such individuals contact with the manasic permanent atom tends to be activated via an aspect of the antahkarana (the Rainbow Bridge).");
    PrintList(3, "~R3B:~ The Business and Active Type of R3. Type 3B is more materially focused than Type 3A, and is characterized by a life of great outer activity - both on the physical plane and in the concrete mind.");
    PrintList(4, "~R4A:~ The Harmony and Beauty Type of R4. Type R4A is more inclined to work sensitively and aesthetically towards harmony than to engage in conflict.");
    PrintList(4, "~R4B:~ The Dramatic, Conflicted Type of R4. Type R4B lives a life of many struggles between opposing forces - both within the environment and within the human energy system. The final tendency may be directed towards the achievement of harmony, but not without considerable dramatic conflict as the forerunner of the harmony to be achieved.");
    PrintList(5, "~R5A:~ The Analytical, Discriminative Type of R5. Type R5A has a keen, analytical, approach to many matters. It is known for its mastery of factual detail and specialized subjects.");
    PrintList(5, "~R5B:~ The Mechanical, Inventive Type of R5. Type R5B is inclined to make practical application of all it knows - especially in the field of mechanics, electronics and, in general, in all technical fields of applied knowledge.");
    PrintList(6, "~R6A:~ The Devoted Type of R6. Type 6A expresses the gentler type of the sixth ray, more expressive of the Neptunian energy. It represents a type of energy which inclines towards deep attachment to the objects of devotion, and to transcendental idealism.");
    PrintList(6, "~R6B:~ The Fiery, Zealous Type of R6. Type 6B expresses forces more correlated with the planet Mars than Neptune. The nature is fierce and often destructive when its ideals are challenged. This type of sixth ray energy can often be involved in the persecution of those who do not share its ideals, visions and creeds and is given to fanaticism.");
    PrintList(7, "~R7A:~ The Highly Organized Type of R7. Type 7A is often correlated with a personality on the seventh ray. The love of correct appearance, pattern, routine and arrangement is pronounced. The power of Saturn is prominent.");
    PrintList(7, "~R7B:~ The Transformative, Renovative Type of R7. Type 7B is more likely to be correlated with the seventh ray `soul` than with the seventh ray personality. `Re`novation and `re`organization are seen to be of greater importance than the maintenance of strict order. The power of Uranus (a largely seventh ray planet) is prominent.");
    EndList();
  } else if (nWhat == 2)
    PrintLongBr("Although there are Seven Rays, they are divided into two main energy streams or lines. The first line derives from R1 and includes R3, R5 and R7; these four are the so-called \"hard line\" rays. The second line (the \"soft line\") derives from R2 and includes R4 and R6. It is often possible to gather an overall impression of oneself or another individual simply by consulting the relative elevation of the two ray lines - the \"hard line\" in red and the \"soft line\" in blue. The hard line is more related to form and the soft line to the life within the form. The general balance of the two ray lines can be reasonably evaluated by consulting these graphs. The general tendency of the two lines to increase or decrease can be seen. You may also decide in a general way whether you should work towards achieving a better balance between them.");

  for (nTime = 0; nTime < 5; nTime++) {
    StartBold();
    Print("  ");
    switch (nWhat) {
    case 0: Print("Rays"); break;
    case 1: Print("Ray types"); break;
    case 2: Print("Ray lines"); break;
    }
    Print(" ");
    switch (nTime) {
    case 0: Print("NOW"); break;
    case 1: Print("FORMERLY"); break;
    case 2: Print("NOW and FORMERLY combined"); break;
    case 3: Print("FORMERLY versus NOW compared"); break;
    case 4: Print("FORMERLY to NOW difference"); break;
    }
    Print(":");
    EndBold();
    PrintBr();

if (nWhat == 0) {
  if (nTime == 0) {
    PrintLongBr("This chart is one of the most important. We see seven elevations for the Seven Rays as they are to be understood `in general`, without taking into consideration the Ray-Types - i.e., the subtypes of any particular ray (and there may be, even, seven subtypes later to be identified). The statements correlated with this chart are meant to test for qualities which all people who strongly express a ray will demonstrate, regardless of Ray-Type.");
    PrintLongBr("To gain an understanding of your rays, RAYS NOW should, however, `not` be studied in isolation. It should be studied along with Ray-Types to gain the best overview. As well, a study of your astrological chart is indispensable - though this is a more advanced consideration.");
    PrintLongBr("RAYS NOW attempts to score your response to ray tendencies `in the present moment` - the moment when you take PIP III. There is always a present moment and a reasonably correct ray configuration for that moment. While the soul ray may change only once (or perhaps, at the most, twice) over your human incarnational history of millions of years, your personality may begin to change in the present incarnation so that towards the last third of your life you may evidence signs of the personality ray you will have in your next incarnation. This is also true for the rays of your personality vehicles. So, it must be understood, that your Ray Formula is in a state of flux during your present incarnation. And each of the vehicles in your energy system has subrays as well.");
    PrintLongBr("It is best to study RAYS NOW as giving an overview of the present strength of the Seven Rays as they manifest `generally` in your energy system at this time. You should not, however, assume that the highest ray is necessarily your soul ray. It may be your personality ray or even the ray of your mind if you are very strongly focussed mentally. It must also be said that \"wishful thinking\" sometimes affects the elevation of a particular bar in the chart. For instance, if you have a deep desire to be strong and steadfast you may respond to the Ray One statements as if you were stronger and more steadfast than you really are. The other general scale, RAYS FORMERLY is meant to help you differentiate between how you `have been` and how you `really are now`.");
    PrintLongBr("Astrological factors can also affect the elevation of the rays. The Seven Rays are transmitted through various astrological signs and planets and so a particular kind of astrological emphasis at the present moment may stimulate or depress the elevation of a certain ray in RAYS NOW independently of your Ray Formula. This is why it is very important to understand esoteric astrology when attempting to gain an accurate assessment of your Ray Chart, your Ray Formula. For a more complete picture, your astrological chart should definitely be considered along with the testimony of PIP III.");
  } else if (nTime == 1) {
    PrintLongBr("This chart is meant to reveal how the Seven Rays expressed through your energy system `at some time past`. We are not looking for your assessment of `any particular time` in your life, but rather a kind of \"subjective average\" which contrasts how you believe you \"used to be\" compared with \"how you believe you are now\". Quite simply, the ray elevations on the graph are meant to show you where you have `been`, compared to where you are `now`. You will, thus, see what rays have, in your estimation, grown stronger, have remained more or less the same, or have been reduced in the strength of their expression. Thus, when comparing RAYS FORMERLY with RAYS NOW, you can see important ray trends in your life, and also, perhaps, identify underlying astrological trends as well.");
  } else if (nTime == 2) {
    PrintLongBr("This chart is useful because it shows how the tendencies of the past (FORMERLY) fuse and blend with the tendencies of the present (NOW). Our present responses and behaviors have many 'carry-overs' from the past; at our stage of evolution, we are rarely free from past tendencies. Though they are of the past, they live in the present. Thus it is interesting to see the result of combining past tendencies with present assessments of attitude, responses and behaviors.");
  } else if (nTime == 3) {
    PrintLongBr("This chart is useful because it shows the Global Ray elevations side by side from FORMERLY through NOW. This is another way of seeing important ~global ray trends~ in this particular incarnation.");
  } else if (nTime == 4) {
    PrintLongBr("This chart is useful in seeing (according to percentage increase or decrease) the degree to which ray strengths have changed over time. The relative strength of the rays is constantly changing during the course of a given incarnation and from one incarnation to another. We all have `~all the rays~`, however the `emphasis` changes, thus changing the emphasis of any given ray relative to the others. We want to know if there is a certain trend to the ray changes we are undergoing or have undergone. Perhaps the ray trend of the next incarnation can also be discerned. Like all disciples, we may want to have more and more of the virtues of the rays (even though we do not feel particularly related to them), but a realistic estimation is called for - not one simply based upon wishful thinking or 'ray idealism'.");
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
          fprintf(file, "Ray %d%s", i, nWhat != 1 ? "" : (t ? "B" : "A"));
        else
          fprintf(file, "%s line Rays", i <= 1 ? "Hard" : "Soft");
        sprintf(sz, "%s:", nTime != 3 ? "" : (f ? " now     " : " formerly"));
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
            Print(fHTML ? " " : "|");
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
          Print(fHTML ? " " : "|");
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

  PrintBanner("HIGHEST ASPIRATIONS");
  PrintLong("While aspirations are often thought to originate in the personality, really, they are stimulated by the magnetic power of the Soul drawing the personality \"upward\". This tabulation will reveal to you your highest values according to Ray-Type. The goals which you rank highest will have much to do with your soul and personality rays. Those you rank lowest may correlate with rays which are not found in your energy system. Sometimes a high aspiration has to do with qualities you idealize but which are not really part of your present energy system. In this case the aspirations are `compensatory`. Sometimes you may find that you no longer aspire to the goals to which you `used to` aspire - at least not to the same extent as in the past. In any case, the ranking of your aspirations should help to confirm your Ray Formula and should correlate with the other aspects of PIP III.");
  StartList();
  for (i = 0; i < cAspire; i++) {
    sprintf(sz, "Aspiration #%d (Ray %d): %s", i+1, rgAspire[i].ray, rgAspire[i].sz);
    PrintList(rgAspire[i].ray, sz);
  }
  EndList();
  StartFixed();
  StartBold();
  PrintLine("RANK            NOW                   FORMERLY");
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
    fprintf(file, "Aspiration #%2d (Ray %d)", n1, rgAspire[n1-1].ray);
    EndColor();
    Print(", ");
    StartColor(rgAspire[n2-1].ray);
    fprintf(file, "Aspiration #%2d (Ray %d)", n2, rgAspire[n2-1].ray);
    EndColor();
    EndFixed();
    PrintBr();
  }
  PrintBr();

  PrintBanner("TRAITS TO TRANSFORM");
  PrintLong("In occultism these traits are called \"glamors\" and \"illusions\". They relate to negative qualities which individuals express when they are strongly influenced by particular rays but unable to control or properly direct the expression of these rays. Energies are neither good nor bad; all depends on the condition of the form through which they work. We all have negative traits which should be transformed into positive traits. These negative expressions should correlate with the ray energies found in our Ray Chart presently, or perhaps formerly - for instance in a previous incarnation. By studying your most prominent \"Traits to Transform\", you can gather important hints regarding the presence of certain rays in your Ray Formula. You will also see, perhaps, which former \"Traits to Transform\" are no longer causing you difficulty. Overall, you may also receive hints on the manner in which you may improve your character.");
  StartList();
  for (i = 0; i < cTrait; i++) {
    sprintf(sz, "Trait #%d (Ray %d): %s", i+1, rgTrait[i].ray, rgTrait[i].sz);
    PrintList(rgTrait[i].ray, sz);
  }
  EndList();
  StartFixed();
  StartBold();
  PrintLine("RANK      NOW              FORMERLY");
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
    fprintf(file, "Trait #%2d (Ray %d)", n1, rgTrait[n1-1].ray);
    EndColor();
    Print(", ");
    StartColor(rgTrait[n2-1].ray);
    fprintf(file, "Trait #%2d (Ray %d)", n2, rgTrait[n2-1].ray);
    EndColor();
    EndFixed();
    PrintBr();
  }
  PrintBr();

  PrintBanner("RESPONSE PATTERNS");
  PrintLongBr("There is high value in this tabulation. Definite response tendencies are indicated and these tendencies can be correlated with the rays. For instance, those with a high R5 in their energy system, may be highly discriminating and rarely use the category Definitely True, because they can always find exceptions. The overall elevations of their ray columns tend to be lower than the elevations for certain other Ray-Types. With R6 types, both the Definitely True and Definitely False category may be strongly emphasized, because they tend to see life in terms of extremes. The ray elevations for R6 types tends to be higher than normal, perhaps because they are `not` so discriminating. They are also to a considerable extent \"front loaded\", which means that the categories DT and UT will have considerable weight. When it comes to R1 types, they may see themselves as appropriating all rays and so, again, we may find high ray elevations. R4 types may have difficulty seeing things all one way or another and so responses that are \"More True that False\", \"In Between\" or \"More False than True\" may be found in abundance. The Ray Response Pattern may also be used to detect patterns of extroversion and introversion, with higher elevations indicating the former (extroversion) and relatively lower elevations the latter (introversion). All this is to say that the Ray Response Pattern should be studied for the clues it may reveal regarding predominating rays. More analysis on this type of tabulation is in process.");
  PrintLongBr("It should also be said that when a ray has a large number in the category Definitely True, that ray may be presumed to be prominent in the energy system. If the same ray that has many Definitely True responses also has some Definitely False responses, the cause is usually to be found in the difference between Ray-Types which often can be of very contrasting natures.");
  for (f = 0; f <= 1; f++)
    for (i = 0; i < cStatement; i++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      rgPattern[n][rgStatement[i].ray][f]++;
      rgPattern[n][0][f]++;
    }
  for (f = 0; f <= 1; f++) {
    PrintHeading(!f ? "NOW" : "FORMERLY");
    StartFixed();
    PrintCh(' ', 6);
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

  PrintBanner("STATEMENT RESPONSES");
  PrintLong("This is a comprehensive tabulation which makes it possible for you to quickly find your response to `any` statement on PIP III. You can easily see how you responded to any given statement with regard to the past (FORMERLY) and to the present (NOW). When pondering the statement involved (from a copy of PIP III) you can review how you felt and thought about that particular statement when you took PIP III and whether you still think and feel that way.");
  StartList();
  for (i = 0; i < cStatement; i++) {
    sprintf(sz, "Statement #%d (Ray %d%s): %s", i+1, rgStatement[i].ray, (rgStatement[i].nType & a) > 0 ? "a" : ((rgStatement[i].nType & b) > 0 ? "b" : ""), rgStatement[i].sz);
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
    PrintHeading(!f ? "NOW" : "FORMERLY");
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

  PrintBanner("END OF RESULTS");
  sprintf(sz, "Thank you for taking the PIP III test. When you completed the test, an e-mail window should have appeared containing your results. Please take a moment to send this message to us at %sAstara@msn.com%s. Your privacy is important, and these results are kept confidential and only used for statistical analysis and research purposes, and to guide future versions of the test. If an e-mail window did not appear, then please manually e-mail us this results file, or rather just copy and paste the contents below:", fHTML ? "<a href=\"mailto:Astara@msn.com\">" : "", fHTML ? "</a>" : "");
  if (!fHTML)
    PrintLongBr(sz);
  else
    fprintf(file, "%s<br>\n", sz);
  if (file != stdout) {
    if (fHTML)
      fprintf(file, "<blockquote>");
    StartFixed();
    StartColor(-128);
    PrintLong("[Begin section of text to e-mail us.]");
    Save(file, fTrue);
    PrintLong("[End section of text to e-mail us.]");
    EndColor();
    EndFixed();
    if (fHTML)
      fprintf(file, "</blockquote>");
    else
      PrintBr();
  }
  sprintf(sz, "The PIP III is offered free of charge, and is supported by voluntary contributions. If you would like to contribute and help continue the work, please visit %shttps://www.sevenray.org/donate/%s. Thank you!", fHTML ? "<a href=\"https://www.sevenray.org/donate/\">" : "", fHTML ? "</a>" : "");
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
      printf("#%d: Ray %d%s%s, %s, %s: %s\n", i+1, ps->ray, (ps->nType & a) > 0 ? "a" : ((ps->nType & b) > 0 ? "b" : ""), (ps->nType & G) > 0 ? " general" : "", ps->fReverse ? "REVERSED" : "normal", ps->fSkip ? "SKIP" : "include", ps->sz);
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

  printf("Welcome to PIP III: The Personal Identity Profile Version 3.\n");
  printf("Copyright (c) 2010-2021 by Michael Robbins and Rick Good. All Rights Reserved.\n");
  printf("PC program release: %s by Walter Pullen (%s).\n", szDate, szAddr);
  printf("Web: https://www.sevenray.org/education/\n\n");

  PrintLongBr("Type your answer to each question then press <Enter>. When you finish the last question, a browser window will appear with your results. If you make a mistake answering, type \"undo\" to back up to the previous question. You can also type \"help\" for a list of other commands to assist with reviewing earlier responses.");

  fileIn = fopen(szSave, "r");
  if (fileIn != NULL) {
    Load(fileIn);
    fclose(fileIn);
LLoaded:
    if (!FFindLatest(&iPart, &i, &f)) {
      printf("NOTICE: You have a test in progress. Type \"restart\" if you want to start over.\n");
      goto LNext;
    } else
      printf("NOTICE: You've completed all answers. Type \"latest\" to display final results.\n");
  }

  for (iPart = 1; iPart <= 4; iPart++) {
LPart:
    if (iPart > 4) {
      printf("ERROR: Can't jump beyond Part 4.\n");
      iPart = 4;
    }
    switch (iPart) {
    case 1: PrintBanner("Part 1/4: Initial questions");   break;
    case 2: PrintBanner("Part 2/4: Statements");          break;
    case 3: PrintBanner("Part 3/4: Highest Aspirations"); break;
    case 4: PrintBanner("Part 4/4: Traits to Transform"); break;
    }
    iMax = rgPart[iPart-1];
    if (iPart == 3)
      PrintLongBr("While aspirations are often thought to originate in the personality, really, they are stimulated by the magnetic power of the Soul drawing the personality \"upward\". Sometimes a high aspiration has to do with qualities you idealize but which are not really part of your present energy system. In this case the aspirations are compensatory. Sometimes you may find that you no longer aspire to the goals to which you used to aspire - at least not to the same extent as in the past.");
    else if (iPart == 4)
      PrintLongBr("In occultism these traits are called \"glamors\" and \"illusions\". They relate to negative qualities which individuals express when they are strongly influenced by particular rays but unable to control or properly direct the expression of these rays. Energies are neither good nor bad; all depends on the condition of the form through which they work. We all have negative traits which should be transformed into positive traits. These negative expressions should correlate with the ray energies found in our Ray Chart presently, or perhaps formerly - for instance in a previous incarnation.");

    for (f = 0; f <= (int)(iPart >= 3); f++)
    for (i = 0; i < iMax; i++) {
LNext:
      iMax = rgPart[iPart-1];
      if (i < 0) {
        printf("ERROR: Can't back up before question #1.\n");
        i = 0;
      }
      if (i >= iMax) {
        printf("ERROR: Can't jump forward beyond question #%d.\n", iMax);
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
        printf("ERROR: Couldn't create autosave file %s!\n", szSave);
      else {
        Save(fileIn, fFalse);
        fclose(fileIn);
      }
    }
  }
  if (!FFindLatest(&iPart, &i, &f)) {
    printf("NOTICE: End of test reached, but some questions not answered yet.\n");
    goto LNext;
  }

LDone:
  printf("NOTICE: End of test reached. All questions answered!\n");

  file = fopen("pip3.txt", "w");
  if (file == NULL)
    printf("ERROR: Couldn't create plain text results file pip3.txt!\n");
  else {
    DoResults();
    fclose(file);
    printf("NOTICE: Created plain text results file pip3.txt\n");
  }

  file = fopen("pip3.htm", "w");
  if (file == NULL)
    printf("ERROR: Couldn't create HTML results file pip3.htm!\n");
  else {
    fHTML = fTrue;
    fprintf(file, "<html><body><font face=\"Times\">\n");
    fprintf(file, "<img src=\"https://sevenray.org/wp-content/uploads/2019/08/sri-logo-nav-210h.png\">\n");
    PrintBr();
    DoResults();
    fprintf(file, "</font></body></html>\n");
    fHTML = fFalse;
    fclose(file);
    printf("NOTICE: Created HTML results file pip3.htm\n");
    if ((int)ShellExecute(NULL, NULL, "pip3.htm", NULL, NULL, SW_SHOW) < 32) {
      printf("ERROR: Couldn't launch HTML browser!\n");
      if ((int)ShellExecute(NULL, NULL, "pip3.txt", NULL, NULL, SW_SHOW) < 32)
        printf("ERROR: Couldn't launch text viewer!\n");
      else
        printf("NOTICE: Launched viewer to display plain text results file pip3.txt\n");
    } else
      printf("NOTICE: Launched browser to display HTML results file pip3.htm\n");
  }

  file = fopen(szSave, "w");
  if (file == NULL)
    printf("ERROR: Couldn't create save file %s!\n", szSave);
  else {
    Save(file, fFalse);
    fclose(file);
    printf("NOTICE: Created save file %s\n", szSave);
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
  sprintf(szMail, "mailto:%s?subject=PIP3%%20results%%20(Please%%20send%%20us%%20this%%20message!)&body=%s", szAddr, sz);
  if ((int)ShellExecute(NULL, NULL, szMail, NULL, NULL, SW_SHOW) < 32)
    printf("ERROR: Couldn't launch e-mail program!\n");
  else
    printf("NOTICE: E-mail message composed. Please send your results to: %s\n", szAddr);

LExit:
  printf("\n");
  PrintLong("Press <Enter> to review your answers from the beginning of the test. You may also close this window to exit the program.");
  GetString(sz);
  FFindLatest(&iPart, &i, &f);
  goto LNext;
  return 0;
}
