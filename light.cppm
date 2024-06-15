export module light;

namespace light {
int charge{};

export void init(int lvl) {
  if (lvl == 1)
    charge = 0;
}

export void draw() {}
} // namespace light
