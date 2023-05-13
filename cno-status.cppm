export module cno:status;
import :entities;
import :moblist;
import jute;

namespace cno::status {
void killed(const mob &src, const mob &tgt) {
  if (is_player(src)) {
    g::update_status("You killed a " + tgt.type->name);
  } else if (is_player(tgt)) {
    g::update_status("A " + src.type->name + " killed you");
  }
}

void poisoned(const mob &src, const mob &tgt) {
  if (is_player(tgt)) {
    g::update_status("A " + src.type->name + " poisons you");
  }
}

void hit(const mob &src, const mob &tgt) {
  if (is_player(src)) {
    g::update_status("You hit a " + tgt.type->name);
  } else if (is_player(tgt)) {
    g::update_status("A " + src.type->name + " hits you");
  }
}

void near_miss(const mob &src, const mob &tgt) {
  if (is_player(src)) {
    g::update_status("You barely miss " + tgt.type->name);
  } else if (is_player(tgt)) {
    g::update_status("A " + src.type->name + " barely misses you");
  }
}

void miss(const mob &src, const mob &tgt) {
  if (is_player(src)) {
    g::update_status("You miss a " + tgt.type->name);
  } else if (is_player(tgt)) {
    g::update_status("A " + src.type->name + " misses you");
  }
}
} // namespace cno::status
