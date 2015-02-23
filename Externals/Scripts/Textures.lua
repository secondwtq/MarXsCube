TEXTURES.DUMMY = BASES.BaseTexture:newObject({ })

function define_tex (filename)
	return TEXTURES.DUMMY:newObject({
		filename = "./Textures/" .. filename, frame_count = 1,
		frame_col = 1, frame_row = 1 })
end

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

TEXTURES.SATELITE = TEXTURES.DUMMY:newObject({
	filename = "./Textures/satelite.png",
	frame_count = 1,
	frame_col = 1,
	frame_row = 1,
})

TEXTURES.DOGE = TEXTURES.DUMMY:newObject({
	filename = "./Textures/doge.png",
	frame_count = 1,
	frame_col = 1,
	frame_row = 1,
})

TEXTURES.JAGUAR = define_tex "jaguar.png"
TEXTURES.HEIGHTFIELD = define_tex "heightfield.png"
TEXTURES.HEIGHTFIELD_TEST = define_tex "heightfield_test.png"
TEXTURES.HEIGHTFIELD_COMPLEX = define_tex "heightfield_complex.png"
TEXTURES.TERRAINBG = define_tex "terrainbg.png"
TEXTURES.WLANDBRUSH = define_tex "wlandbrush.png"

TEXTURES.TILESET = define_tex "tileset.png"