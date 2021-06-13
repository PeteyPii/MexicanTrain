extends Node2D

var tiles = []
const Tile = preload("res://scenes/Tile.tscn")

export var margin = Vector2(10, 10)
export var tile_separation = 4


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

	var tile_size = tile.get_size()
	var offset = margin + tile_size / 2

	if len(tiles) % 2 == 0:
		# Tile is on the right
		offset.x += tile_separation + tile_size.x
		offset.y += (tile_size.y + tile_separation) / 2

	offset.y += ((len(tiles) - 1) / 2) * (tile_size.y + tile_separation)

	tile.set_position(offset)
