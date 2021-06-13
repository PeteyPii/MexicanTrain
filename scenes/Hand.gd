extends Node2D

const Tile = preload("res://scenes/Tile.tscn")

export var margin = Vector2(10, 10)
export var tile_separation = 4

var tiles = []
var hovered_tile = null
var dragging = false
var dragged_offset = Vector2()
var dragged_tile_index = -1
var original_dragged_tile_index = -1

export var top_y_rearrange_threshold = -100
export var bottom_y_rearrange_threshold = 200

const TOP_MOST_Z_INDEX = 999


func _ready():
	add_tile(Tile.instance().set_pips(1, 2))
	add_tile(Tile.instance().set_pips(3, 4))
	add_tile(Tile.instance().set_pips(5, 6))
	add_tile(Tile.instance().set_pips(7, 8))
	add_tile(Tile.instance().set_pips(9, 10))
	add_tile(Tile.instance().set_pips(11, 12))
	add_tile(Tile.instance().set_pips(13, 14))
	add_tile(Tile.instance().set_pips(15, 0))


func add_tile(tile):
	add_child(tile)
	tiles.append(tile)

	set_tile_position(tile, len(tiles) - 1)

	tile.get_node("Control").connect("mouse_entered", self, "_on_tile_mouse_entered", [tile])
	tile.get_node("Control").connect("mouse_exited", self, "_on_tile_mouse_exited", [tile])
	tile.get_node("Control").connect("gui_input", self, "_on_tile_gui_input", [tile])


func set_tile_position(tile, index):
	if dragging and tile == hovered_tile:
		return
	var tile_size = tile.get_size()
	var offset = margin + tile_size / 2
	offset.x += (tile_separation + tile_size.x) * index
	tile.set_position(offset)
	tile.z_index = index


func insert_tile_sorted(tile):
	var index = tiles.bsearch_custom(tile, self, "_tile_compare")
	tiles.insert(index, tile)
	reflow_tiles()
	return index


func _tile_compare(tile, search_tile):
	return tile.position.x < search_tile.position.x


func reflow_tiles():
	for i in range(len(tiles)):
		set_tile_position(tiles[i], i)


func _input(event):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT:
			if event.pressed && hovered_tile != null:
				dragging = true
				dragged_offset = hovered_tile.position - event.position
				dragged_tile_index = tiles.find(hovered_tile)
				original_dragged_tile_index = dragged_tile_index
				hovered_tile.z_index = TOP_MOST_Z_INDEX
			else:
				if dragging && hovered_tile != null:
					dragging = false
					set_tile_position(hovered_tile, dragged_tile_index)
					dragged_tile_index = -1
					original_dragged_tile_index = -1
	elif event is InputEventMouseMotion:
		if dragging:
			hovered_tile.set_position(event.position + dragged_offset, true)
			var y_diff = hovered_tile.position.y - position.y
			print(y_diff)
			if y_diff < top_y_rearrange_threshold || y_diff > bottom_y_rearrange_threshold:
				if hovered_tile != tiles[original_dragged_tile_index]:
					tiles.remove(dragged_tile_index)
					tiles.insert(original_dragged_tile_index, hovered_tile)
					dragged_tile_index = original_dragged_tile_index
					reflow_tiles()
			elif (
				dragged_tile_index > 0
				&& hovered_tile.position.x < tiles[dragged_tile_index - 1].position.x
			):
				tiles.remove(dragged_tile_index)
				dragged_tile_index = insert_tile_sorted(hovered_tile)
			elif (
				dragged_tile_index < len(tiles) - 1
				&& hovered_tile.position.x > tiles[dragged_tile_index + 1].position.x
			):
				tiles.remove(dragged_tile_index)
				dragged_tile_index = insert_tile_sorted(hovered_tile)


func _on_tile_gui_input(event, tile):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_RIGHT && event.pressed:
			tile.set_flipped(! tile.is_flipped)


func _on_tile_mouse_entered(tile):
	hovered_tile = tile


func _on_tile_mouse_exited(_tile):
	hovered_tile = null
