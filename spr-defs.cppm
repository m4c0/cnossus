export module spr:defs;
import :ids;

export auto attack_of(spr::id item) {
  switch (item) {
  case spr::knife:
    return 2;
  case spr::sickle:
    return 3;
  case spr::adze:
    return 5;
  case spr::axe:
    return 8;
  case spr::doubleaxe:
    return 10;
  case spr::sword:
    return 11;
  case spr::spear:
    return 15;
  default:
    return 1;
  }
}

export auto defense_of(spr::id item) {
  switch (item) {
  case spr::greave:
  case spr::armguard:
  case spr::pauldron:
    return 1;
  case spr::scale:
    return 3;
  case spr::shield:
    return 5;
  default:
    return 0;
  }
}

export auto life_of(spr::id id) {
  switch (id) {
  case spr::scorpion:
  case spr::spider:
    return 4;
  case spr::rat:
  case spr::centipede:
    return 6;
  case spr::boar:
  case spr::cerberus:
  case spr::harpy:
  case spr::snake:
    return 8;
  case spr::bull:
  case spr::chimera:
  case spr::griffin:
  case spr::manticore:
  case spr::sphinx:
    return 12;
  case spr::crocodile:
  case spr::drakon:
    return 20;
  default:
    return 0;
  }
}

export auto poison_of(spr::id e) {
  switch (e) {
  case spr::snake:
  case spr::spider:
    return 4;
  case spr::centipede:
  case spr::scorpion:
    return 8;
  case spr::chimera:
    return 10;
  default:
    return 0;
  }
}

export auto food_of(spr::id id) {
  switch (id) {
  case spr::driedfruit:
    return 3;
  case spr::cheese:
    return 5;
  case spr::rawmeat:
    return 8;
  default:
    return 0;
  }
}
export auto light_of(spr::id id) {
  switch (id) {
  case spr::torch:
    return 10;
  case spr::candle:
    return 15;
  case spr::oillamp:
    return 25;
  default:
    return 0;
  }
}
