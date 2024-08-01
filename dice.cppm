export module dice;
import rng;
import sfx;

// TODO: think a bit more about relabancing
//
// Currently, low-level enemies will die with a one hit kill and got zero
// chances of damage the player. This creates a false sense of safety BUT it
// might scare new players because the game feels "too easy" or even broken.

static int dice(int num) {
  constexpr const auto sides = 2; // More like a coin
  int roll = 0;
  for (auto i = 0; i < num; i++) {
    roll += rng::rand_i(sides);
  }
  return roll;
}
export unsigned attack_roll(int atk_dice, int def_dice) {
  auto atk_roll = dice(atk_dice);
  auto def_roll = dice(def_dice);
  auto roll = atk_roll - def_roll;
  if (roll <= 0) {
    sfx::attack_miss();
    return 0;
  } else {
    sfx::enemy_take_hit();
    return rng::rand_i(atk_dice - def_dice);
  }
}
