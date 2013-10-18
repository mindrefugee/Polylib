Polylib {
	sphere{
		filepath="sphere.stl"
		
	}
	car {
		class_name = "CarGroup"
	//	filepath = "/foo/bar/car.stl_a"
	//		filepath = "./car.stla"
			filepath = "./car.stl"
		movable = "true"
		velocity = 0.05
	}
	windmill {
		class_name  = "PolygonGroup"
		blades {
			class_name = "PolygonGroup"
			blade1 {
				//class_name = "BladeGroup"
				filepath = "./blade1.stl"
				movable = "true"
				center_x = 0.0
				center_y = 123.45
				center_z = 345.67
			}
			blade2 {
				//class_name = "BladeGroup"
				filepath = "./blade2.stl"
				movable = "true"
				center_x = 0.0
				center_y = 123.45
				center_z = 345.67
			}
			blade3 {
				//class_name = "BladeGroup"
				filepath = "./blade3.stl"
				movable = "true"
				center_x = 0.0
				center_y = 123.45
				center_z = 345.67
			}
		}
		tower {
			class_name = "PolygonGroup"
			filepath = "./tower.stl"
		}
	}
} // end of Polylib