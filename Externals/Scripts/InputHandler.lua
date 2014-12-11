-- this file is part of Project MarXsCube Prototype.
--
--	use in other modules:
--
--		InputHandler = require 'InputHandler'
--
--	http://github.com/secondwtq/MarXsCube

local InputHandler = { }

local Helpers = require 'Helpers'

function InputHandler.MousePress_OnCell(mouse_status)
	local coord = Utility.GetCoordFromPoint(mouse_status.pos)
	print(coord.x, coord.y)
	-- local rt_start = coord
	-- local rt_end = Utility.CoordStruct(coord.x+6000, coord.y+6000, coord.z+5000)
	-- local ray_cell = Physics.RayTestSingleForCell.createRayTestForCell(rt_end, rt_start)
	-- ray_cell:perform()

	-- if ray_cell:hit() then
	-- 	local hit_point = ray_cell:hit_point()
	-- 	print(hit_point.x, hit_point.y, hit_point.z)

	-- 	local nearest_node = find_nearest_node(hit_point, 64)
	-- 	if nearest_node then
	-- 		print('Selected node ', nearest_node.components.a['GraphNodeStore']:get_datafield 'idx_initial')
	-- 		if CURRENT_STATUS == "SELECT_FIRST" then
	-- 			PATH_STARTNODE = nearest_node
	-- 			CURRENT_STATUS = "SELECT_SECOND"
	-- 		elseif CURRENT_STATUS == "SELECT_SECOND" then
	-- 			PATH_ENDNODE = nearest_node
	-- 			CURRENT_STATUS = "SELECT_FIRST"

	-- 			local bf_shortest = Appins.Gmap.bellman_ford_shortest(GRAPH_GLOBAL, PATH_STARTNODE.components.a['GraphNodeStore']:get_datafield 'idx_initial')
	-- 			bf_shortest:go()
	-- 			local path_nodes = Appins.Gmap.bellman_ford_shortest.extract_path(bf_shortest, PATH_ENDNODE.components.a['GraphNodeStore']:get_datafield 'idx_initial')

	-- 			for i, v in ipairs(path_nodes) do
	-- 				if i < #path_nodes then
	-- 					-- A structure: find_and_execute_once
	-- 					for _i, edge in ipairs(OBJ_EDGES) do
	-- 						if edge.components.a['GraphLineStore']:connect(v, path_nodes[i+1]) then
	-- 							edge.components.a['RenderElementsManger']:get_element_named 'line'.color1 = Utility.Homogeneous4D(1.0, 0.0, 0.0, 1.0)
	-- 							edge.components.a['RenderElementsManger']:get_element_named 'line'.color2 = Utility.Homogeneous4D(1.0, 0.0, 0.0, 1.0)
	-- 							break
	-- 						end
	-- 					end
	-- 				end
	-- 			end

	-- 			PATH_STARTNODE, PATH_ENDNODE = nil
	-- 		end
	-- 	end
	-- end

end

return InputHandler