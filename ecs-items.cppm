export module ecs:items;
import :ec;
import :entities;
import spr;

export namespace ecs {
// There are multiple differences between the names in the original code, the
// port to iOS and the sprites. So, this is just a mess... :D

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// These three are missing in previous port, but they where in original ZX
// code. Then missing parts can be added or these can be entirely removed
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// linen tunic
auto add_cloth(ecs::ec *ec) { return add_armour_item(ec, spr::cloth, 1); }
// leather vest
auto add_leather(ecs::ec *ec) { return add_armour_item(ec, spr::leather, 2); }
// short sword
auto add_sword(ecs::ec *ec) { return add_weapon_item(ec, spr::sword, 11); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// The bag expands inventory
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

auto add_bag(ecs::ec *ec) { return add_bag_item(ec, spr::bag); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Weapons. Only the best one is kept in inventory
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// rust knife
auto add_knife(ecs::ec *ec) { return add_weapon_item(ec, spr::knife, 2); }
auto add_sickle(ecs::ec *ec) { return add_weapon_item(ec, spr::sickle, 3); }
auto add_adze(ecs::ec *ec) { return add_weapon_item(ec, spr::adze, 5); }
auto add_axe(ecs::ec *ec) { return add_weapon_item(ec, spr::axe, 8); }
// battle axe
auto add_doubleaxe(ecs::ec *ec) {
  return add_weapon_item(ec, spr::doubleaxe, 10);
}
auto add_spear(ecs::ec *ec) { return add_weapon_item(ec, spr::spear, 15); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Food restores life
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// strange fruit
auto add_driedfruit(ecs::ec *ec) {
  return add_food_item(ec, spr::driedfruit, 3);
}
auto add_cheese(ecs::ec *ec) { return add_food_item(ec, spr::cheese, 5); }
auto add_rawmeat(ecs::ec *ec) { return add_food_item(ec, spr::rawmeat, 8); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Light sources scare monsters
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

auto add_torch(ecs::ec *ec) { return add_light_item(ec, spr::torch, 10); }
auto add_candle(ecs::ec *ec) { return add_light_item(ec, spr::candle, 15); }
// lamp
auto add_oillamp(ecs::ec *ec) { return add_light_item(ec, spr::oillamp, 25); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Armour. In the original game, limb protection (gloves, boots, etc)
// could be carried in pairs.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// boots
auto add_greave(ecs::ec *ec) { return add_armour_item(ec, spr::greave, 1); }
// gloves
auto add_armguard(ecs::ec *ec) { return add_armour_item(ec, spr::armguard, 1); }
// helmet
auto add_pauldron(ecs::ec *ec) { return add_armour_item(ec, spr::pauldron, 1); }
// chest armour
auto add_scale(ecs::ec *ec) { return add_armour_item(ec, spr::scale, 3); }
auto add_shield(ecs::ec *ec) { return add_armour_item(ec, spr::shield, 5); }
} // namespace ecs
