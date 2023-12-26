namespace c_Tests
{
	[TestClass]
	public class InitializationTests
	{
		WaveFunctionCollapse_CPP dll = new();

		[TestMethod]
		public void TestMethod1()
		{
			//test.
		}

		public void CreateRules()
		{
			const ulong GRASS = 1 << 0;
			const ulong SAND = 1 << 1;
			const ulong WATER = 1 << 2;

			WFCPosition[] IgnoreEast = new WFCPosition[] { new WFCPosition(-1, 1), new WFCPosition(-1, 0), new WFCPosition(-1, -1) };
			WFCPosition[] IgnoreWest = new WFCPosition[] { new WFCPosition(1, 1), new WFCPosition(1, 0), new WFCPosition(1, -1) };
			WFCPosition[] IgnoreAll = new WFCPosition[] {	new WFCPosition(-1, 1), new WFCPosition(0, 1)	, new WFCPosition(1, 1),
															new WFCPosition(-1, 0)	   /*Tile goes here*/	, new WFCPosition(1, 1),
															new WFCPosition(-1, -1), new WFCPosition(0, 1)	, new WFCPosition(1, 1) };

			//Grass to sand
			WaveFunctionCollapse_CPP.CellIsNotRule ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();

			ruleToAdd.tile = GRASS;
			ruleToAdd.goal = SAND;
			ruleToAdd.localTargets = IgnoreEast;

			dll.AddCellIsNotRule(ruleToAdd);

			ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();

			ruleToAdd.tile = GRASS;
			ruleToAdd.goal = WATER;
			ruleToAdd.localTargets = new WFCPosition[] { new WFCPosition(-1, 1), new WFCPosition(-1, 0), new WFCPosition(-1, -1) };

			dll.AddCellIsNotRule(ruleToAdd);

			ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();

			ruleToAdd.tile = SAND;
			ruleToAdd.goal = GRASS;
			ruleToAdd.localTargets = IgnoreEast;

			dll.AddCellIsNotRule(ruleToAdd);

			ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();

			ruleToAdd.tile = SAND;
			ruleToAdd.goal = WATER;
			ruleToAdd.localTargets = IgnoreWest;

			dll.AddCellIsNotRule(ruleToAdd);

			ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();

			ruleToAdd.tile = WATER;
			ruleToAdd.goal = SAND;
			ruleToAdd.localTargets = IgnoreEast;

			dll.AddCellIsNotRule(ruleToAdd);

			ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();

			ruleToAdd.tile = WATER;
			ruleToAdd.goal = GRASS;
			ruleToAdd.localTargets = IgnoreAll;

			dll.AddCellIsNotRule(ruleToAdd);
		}

		[TestMethod]
		public void TestCreateRules()
		{
			CreateRules();
			Assert.IsTrue(true);
		}
		/* dll ??= new WaveFunctionCollapse_CPP();
		DrawLine(20, 20);

		if (GUILayout.Button("Import to DLL"))
		{
			ulong tileIDs = 0;
			WFCTile[] toAdd = importer.Import<string>("Needed to provide some type");
			foreach (WFCTile tile in toAdd)
			{
				tileIDs += tile.ID;
				foreach (WFCRule rule in tile.Rules)
				{
					if (rule is MultiCellIsNotTarget2D)
					{
						MultiCellIsNotTarget2D multiTargetRule = rule as MultiCellIsNotTarget2D;
						WaveFunctionCollapse_CPP.CellIsNotRule ruleToAdd = new WaveFunctionCollapse_CPP.CellIsNotRule();
						ruleToAdd.tile = tile.ID;
						ruleToAdd.goal = multiTargetRule.goal.ID;
						ruleToAdd.localTargets = multiTargetRule.GetTargetCellsArray();
						dll.AddCellIsNotRule(ruleToAdd);
					}
				}
			}

			Debug.Log("Imported successfully");
		}


		if (GUILayout.Button("Initialize and set Size to (5,5)") && dll != null)
		{
			dll.Create2DWFC(new WFCPosition(5, 5));
			Debug.Log("Initialized successfully");
		}

		if (GUILayout.Button("collapse specific cell to sand DLL") && dll != null)
		{
			dll.CollapseSpecificCell(4, new WFCPosition(2, 2));
			Debug.Log("Collapsed Specific Cell successfully");
		}

		if (GUILayout.Button("Run DLL") && dll != null)
		{
			dll.RunGenerator();
			Debug.Log("Run the Generator successfully");
		}

		if (GUILayout.Button("Export DLL Results") && dll != null)
		{
			ulong[] test = dll.GetResults();
			Debug.Log("Exported from the Generator successfully, recieved " + test.Length + "results");
		}
		*/
	}
}