export module spr;
import dotz;
import quack;

export namespace spr {
enum id : char {
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

  cloth = 'a',
  leather = 'b',
  sword = 'c',
  bag = 'd',
  knife = 'e',
  sickle = 'f',
  adze = 'g',
  axe = 'h',
  doubleaxe = 'i',
  spear = 'j',
  driedfruit = 'k',
  cheese = 'l',
  rawmeat = 'm',
  torch = 'n',
  candle = 'o',
  oillamp = 'p',
  greave = 'q',
  armguard = 'r',
  pauldron = 's',
  scale = 't',
  shield = 'u',
  jar = 'v',
  coffer = 'w',

  basin = '$',
  column = '#',
  exit = '<',
  floor = '.',
  mosaic = ',',
  pool = '%',
  statue = '(',
  wall = '&',
};

unsigned blit(id i, float x, float y, quack::mapped_buffers &all) {
  if (i == 0)
    return 0;

  auto uv = dotz::vec2{i % 16, i / 16} / 16.0;

  auto &[c, m, p, u] = all;
  *c++ = {1, 1, 1, 1};
  *m++ = {1, 1, 1, 1};
  *p++ = {{x, y}, {1, 1}};
  *u++ = {uv, uv + 1.0 / 16.0};
  return 1;
}
}; // namespace spr
