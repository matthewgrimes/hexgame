#pragma once
enum Component {
  COMPONENT_NONE = 0,
  COMPONENT_MESH = 1 << 0,
  COMPONENT_NAME = 1 << 1,
  COMPONENT_WORLD_POS = 1 << 2
};