export module inv:domain;
import ecs;

export namespace inv::items {
// There are multiple differences between the names in the original code, the
// port to iOS and the sprites. So, this is just a mess... :D

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// These three are missing in previous port, but they where in original ZX
// code. Then missing parts can be added or these can be entirely removed
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// linen tunic
void add_cloth(ecs::ec *ec) { add_armour_item(ec, 'a', 1); }
// leather vest
void add_leather(ecs::ec *ec) { add_armour_item(ec, 'b', 2); }
// short sword
void add_sword(ecs::ec *ec) { add_weapon_item(ec, 'c', 11); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// The bag expands inventory
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void add_bag(ecs::ec *ec) { add_bag_item(ec, 'd'); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Weapons. Only the best one is kept in inventory
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// rust knife
void add_knife(ecs::ec *ec) { add_weapon_item(ec, 'e', 2); }
void add_sickle(ecs::ec *ec) { add_weapon_item(ec, 'f', 3); }
void add_adze(ecs::ec *ec) { add_weapon_item(ec, 'g', 5); }
void add_axe(ecs::ec *ec) { add_weapon_item(ec, 'h', 8); }
// battle axe
void add_doubleaxe(ecs::ec *ec) { add_weapon_item(ec, 'i', 10); }
void add_spear(ecs::ec *ec) { add_weapon_item(ec, 'j', 15); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Food restores life
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// strange fruit
void add_driedfruit(ecs::ec *ec) { add_food_item(ec, 'k', 3); }
void add_cheese(ecs::ec *ec) { add_food_item(ec, 'l', 5); }
void add_rawmeat(ecs::ec *ec) { add_food_item(ec, 'm', 8); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Light sources scare monsters
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void add_torch(ecs::ec *ec) { add_light_item(ec, 'n', 10); }
void add_candle(ecs::ec *ec) { add_light_item(ec, 'o', 15); }
// lamp
void add_oillamp(ecs::ec *ec) { add_light_item(ec, 'p', 25); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Armour. In the original game, limb protection (gloves, boots, etc)
// could be carried in pairs.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// boots
void add_greave(ecs::ec *ec) { add_armour_item(ec, 'q', 1); }
// gloves
void add_armguard(ecs::ec *ec) { add_armour_item(ec, 'r', 1); }
// helmet
void add_pauldron(ecs::ec *ec) { add_armour_item(ec, 's', 1); }
// chest armour
void add_scale(ecs::ec *ec) { add_armour_item(ec, 't', 3); }
void add_shield(ecs::ec *ec) { add_armour_item(ec, 'u', 5); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Item containers. When interacted, they drop a random item
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void add_jar(ecs::ec *ec) { add_container_item(ec, 'u'); }
// chest
void add_coffer(ecs::ec *ec) { add_container_item(ec, 'v'); }

} // namespace inv::items
