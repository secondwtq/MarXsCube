function Functions.TestManger_onTestInit()
	Objects.Map.GetInstance():CreateEmptyMap(50, 50)
	ModEnvironment.Functions.createAnim(OBJECTS.TESTANIM, Utility.CoordStruct(1024, 512, 512))

	local Techno = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(1560, 126, 0), true)
	Techno.Physics:setDirection(5)

	Techno = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(256, 512, 0), true)
	Techno.Physics:setDirection(6)

	Techno = ModEnvironment.Functions.createTechno(OBJECTS.TESTTECHNO, Utility.CoordStruct(400, 600, 0), true)

	local TestTechnoTypePhy = OBJECTS.TESTTECHNO_PHY:InitialType()
	Techno = Objects.Type_Techno.createTechno(TestTechnoTypePhy)
	Techno.EnablePhysics = true;
	Techno:SpawnAtMapCoord(Utility.CoordStruct(1000, 126, 0))
	Techno.ExtTable.isDisabled = true
	Techno.Physics:setDirection(0)

	ModEnvironment.Functions.createTechno(OBJECTS.TESTBUILDING, Utility.CoordStruct(0, 0, 0), true)
end