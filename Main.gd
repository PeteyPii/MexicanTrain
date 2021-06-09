extends Node2D


func _init():
  print("hello")
  var l = load("res://SimpleClass.gdns")
  var data = l.new()
  print(data.method("haha"))
  print(data.method([]))


func _idle(_delta):
  print("hello2")
  return true
