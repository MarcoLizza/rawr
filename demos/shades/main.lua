local Input = require("tofu.events").Input
local Bank = require("tofu.graphics").Bank
local Canvas = require("tofu.graphics").Canvas
local Font = require("tofu.graphics").Font
local Class = require("tofu.util").Class
local System = require("tofu.core").System

local PALETTE
local STEPS
local LEVELS
local TARGET = 0x00000000

local function get_r(c)
  return (c >> 16) & 0xff
end
local function get_g(c)
  return (c >> 8) & 0xff
end
local function get_b(c)
  return c & 0xff
end

local function find_best_match(palette, match)
  local index
  local min = math.huge
  for i, color in ipairs(palette) do
    local dr, dg, db = (get_r(match) - get_r(color)), (get_g(match) - get_g(color)), (get_b(match) - get_b(color))
    local d = dr * dr * 2.0 + dg * dg * 4.0 + db * db * 3.0
    if min > d then
      min = d
      index = i
    end
  end
  return index
end

local function build_table(palette, levels, target)
  local lut = {}
  for i = 0, levels - 1 do
    local shifting = {}
    local ratio = 1 / (levels - 1) * i
    for j, color in ipairs(palette) do
      local r = math.tointeger((get_r(color) - get_r(target)) * ratio + get_r(target))
      local g = math.tointeger((get_g(color) - get_g(target)) * ratio + get_g(target))
      local b = math.tointeger((get_b(color) - get_b(target)) * ratio + get_b(target))
      local k = find_best_match(palette, r * 65536 + g * 256 + b)
      shifting[j - 1] = k - 1
    end
    lut[i] = shifting
  end
  return lut
end

local Main = Class.define()

function Main:__ctor()
  Canvas.palette("pico-8")

  PALETTE = Canvas.palette()
  STEPS = #PALETTE
  LEVELS = STEPS

  self.lut = build_table(PALETTE, LEVELS, TARGET)

  self.bank = Bank.new("assets/sheet.png", 8, 8)
  self.font = Font.default(0, 15)
  self.width = Canvas.width() / STEPS
  self.height = Canvas.height() / STEPS
  self.mode = 0
end

function Main:input()
  if Input.is_pressed(Input.Y) then
    self.mode = (self.mode + 1) % 10
  end
end

function Main:update(_)
end

function Main:render(_)
  Canvas.clear()

  for i = 0, STEPS - 1 do
    local y = self.height * i
    for j = 0, STEPS - 1 do
      local x = self.width * j
      Canvas.rectangle("fill", x, y, self.width, self.height, i + j)
    end
  end

  Canvas.push()
  Canvas.transparent({ [0] = false })
  if self.mode == 0 then
    for i = 0, STEPS - 1 do
      local y = self.height * i
      Canvas.shift(self.lut[i])
      Canvas.process(0, y, Canvas.width(), self.height)
    end
  elseif self.mode == 1 then
    for i = 0, STEPS - 1 do
      Canvas.shift(self.lut[i])
      Canvas.process(i, 0, 1, Canvas.height())
      Canvas.process(Canvas.width() - 1 - i, 0, 1, Canvas.height())
    end
  else
    local t = System.time()
    local index = math.tointeger((math.sin(t * 2.5) + 1) * 0.5 * (STEPS - 1))
    Canvas.shift(self.lut[index])
    Canvas.process(0, 0, Canvas.width(), Canvas.height() / 2)
  end
  Canvas.pop()

  self.font:write(string.format("FPS: %d", System.fps()), 0, 0, "left")
end

return Main
