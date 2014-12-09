TEXTURES.DUMMY = BASES.BaseTexture:newObject({ })

TEXTURES.TESTANIM_TEX = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TESTANIM.png",
	frame_count = 45,
	frame_row = 7,
	frame_col = 7,
	useExtraTexture = true,
	extra_num = 1,
	extra = {
		{ 45, "./Textures/TESTANIM.png" },
	}
})

TEXTURES.TESTDDS_TEX = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TESTANIM2.tga",
	frame_count = 28,
	frame_row = 8,
	frame_col = 4,
})

TEXTURES.TESTANIM_TEX_FULL = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TESTANIM.png",
	frame_count = 1,
	frame_row = 1,
	frame_col = 1,
})

TEXTURES.TESTUNITBODYIMAGE = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TestUnit.png",
	frame_count = 32,
	frame_col = 6,
	frame_row = 6,
})

TEXTURES.TESTUNITSHADOWIMAGE = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TestShadow.png",
	frame_count = 32,
	frame_col = 6,
	frame_row = 6,
})

TEXTURES.TESTBUILDING_BODY = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TESTBUILDING_BODY.png",
	frame_count = 1,
	frame_col = 1,
	frame_row = 1,
})

TEXTURES.TESTBUILDING_SHADOW = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TESTBUILDING_SHADOW.png",
	frame_count = 1,
	frame_col = 1,
	frame_row = 1,
})

TEXTURES.TRIKEIMAGE = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TRIKEIMAGE.png",
	frame_count = 36,
	frame_col = 6,
	frame_row = 6,
})

TEXTURES.TRIKEIMAGE_SHADOW = TEXTURES.DUMMY:newObject({
	filename = "./Textures/TRIKEIMAGE_SHADOW.png",
	frame_count = 36,
	frame_col = 6,
	frame_row = 6,
})

TEXTURES.GRAPH_NODE_IMAGE = TEXTURES.DUMMY:newObject({
	filename = "./Textures/tex_graph_node.png",
	frame_count = 1,
	frame_col = 1,
	frame_row = 1,
})

TEXTURES.GRAPH_NODE_SHADOW = TEXTURES.DUMMY:newObject({
	filename = "./Textures/tex_graph_node_shadow.png",
	frame_count = 1,
	frame_col = 1,
	frame_row = 1,
})

-- TEXTURES.TESTBIG = TEXTURES.DUMMY:newObject({
-- 	filename = ".\\Textures\\BIG.png",
-- 	frame_count = 32,
-- 	frame_col = 6,
-- 	frame_row = 6,
-- })