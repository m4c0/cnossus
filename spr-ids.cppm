export module spr:ids;

// There are multiple differences between the names in the original code, the
// port to iOS and the sprites. There is a bunch of mismatches, like "rat"
// having a bat sprite, "boar" using a rat sprite, "linen tunic" renamed as
// "cloth", etc.
//
// Some basic rules:
// * bag expands inventory
// * food restore life
// * weapon - keep the best attack
// * armour - one per body part. The original game supports pairs for limbs
//            (gloves, boots, etc)
// * light changes monster patterns

export namespace spr {
enum id : unsigned char {
  nil = 0,

  // UI elements
  cursor = 1,
  ui_bar_b = 9,
  ui_bar,
  ui_bar_t,
  ui_check,

  // Mobs
  minotaur = 'A',
  snake = 'B',
  spider = 'C',
  rat = 'D',
  scorpion = 'E',
  centipede = 'F',
  cerberus = 'G',
  harpy = 'H',
  chimera = 'I',
  manticore = 'J',
  crocodile = 'K',
  drakon = 'L',
  boar = 'M',
  griffin = 'N',
  sphinx = 'O',
  bull = 'P',

  // Items
  // "linen tunic", armour, 1 - missing in my iOS version
  cloth = 'a',
  // "leather vest", armour, 2 - missing in my iOS version
  leather = 'b',
  // "short sword", weapon, 11 - missing in my iOS version
  sword = 'c',
  bag = 'd',
  // "rust knife"
  knife = 'e',
  sickle = 'f',
  adze = 'g',
  axe = 'h',
  // "battle axe"
  doubleaxe = 'i',
  spear = 'j',
  // "strange fruit"
  driedfruit = 'k',
  cheese = 'l',
  rawmeat = 'm',
  torch = 'n',
  candle = 'o',
  // "lamp"
  oillamp = 'p',
  // "boots"
  greave = 'q',
  // "gloves"
  armguard = 'r',
  // "helmet"
  pauldron = 's',
  // "chest armour"
  scale = 't',
  shield = 'u',
  jar = 'v',
  coffer = 'w',

  // Map
  basin = '$',
  column = '#',
  exit = '<',
  floor = '.',
  mosaic = ',',
  pool = '%',
  statue = '(',
  wall = '&',
};
}; // namespace spr
