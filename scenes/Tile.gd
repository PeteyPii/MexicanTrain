extends Node2D

export var is_flipped = false
export var is_face_down = false
export var top_pips = 0
export var bottom_pips = 0

const SPRITE_SHEET_GRID_SIZE = 4

const PIPS_TO_COLOR = {
	0: Color("ffffff"),
	1: Color("292929"),
	2: Color("9b9b9b"),
	3: Color("270437"),
	4: Color("9b9b9b"),
	5: Color("301dd7"),
	6: Color("fffd6f"),
	7: Color("8a751f"),
	8: Color("14500d"),
	9: Color("209bdd"),
	10: Color("e7931d"),
	11: Color("d7243d"),
	12: Color("c62d6a"),
	13: Color("5e2f09"),
	14: Color("e97fea"),
	15: Color("603896"),
}


func _init(top_pips_ = 0, bottom_pips_ = 0):
	self.top_pips = top_pips_
	self.bottom_pips = bottom_pips_


func _ready():
	var sprite_sheet_size = $TopPips.texture.get_size()

	assert(sprite_sheet_size.x == sprite_sheet_size.y)
	var scale = sprite_sheet_size.x / SPRITE_SHEET_GRID_SIZE

	$TopPips.region_rect.size = Vector2(scale, scale)
	$TopPips.region_rect.position = Vector2(
		scale * (self.top_pips % SPRITE_SHEET_GRID_SIZE),
		scale * (self.top_pips / SPRITE_SHEET_GRID_SIZE)
	)
	$TopPips.modulate = PIPS_TO_COLOR[self.top_pips]

	$BottomPips.region_rect.size = Vector2(scale, scale)
	$BottomPips.region_rect.position = Vector2(
		scale * (self.bottom_pips % SPRITE_SHEET_GRID_SIZE),
		scale * (self.bottom_pips / SPRITE_SHEET_GRID_SIZE)
	)
	$BottomPips.modulate = PIPS_TO_COLOR[self.bottom_pips]

	set_flipped(self.is_flipped)
	set_face_down(self.is_face_down)


func set_flipped(is_flipped_):
	self.is_flipped = is_flipped_

	if self.is_flipped:
		rotation_degrees = 180
	else:
		rotation_degrees = 0


var face_up_tex = preload("res://assets/textures/domino_front.png")
var face_down_tex = preload("res://assets/textures/domino_back.png")


func set_face_down(is_face_down_):
	self.is_face_down = is_face_down_

	if self.is_face_down:
		$TopPips.visible = false
		$BottomPips.visible = false
		$Surface.texture = face_down_tex
	else:
		$TopPips.visible = true
		$BottomPips.visible = true
		$Surface.texture = face_up_tex
