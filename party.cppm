export module party;

import hai;
import qsu;

namespace party {
struct emitter {};
struct particle {};

hai::varray<emitter> emitters{};
hai::varray<particle> particles{};

export void add_emitter(emitter e) { emitters.push_back_doubling(e); }

export void tick() {}
} // namespace party
