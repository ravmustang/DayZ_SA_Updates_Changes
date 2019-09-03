class Apple : Edible_Base
{
	override bool CanBeCooked()
	{
		return true;
	}
	
	override bool CanBeCookedOnStick()
	{
		return true;
	}	
	
	override bool IsFruit()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionForceFeed);
		AddAction(ActionEatFruit);
	}
	
	override void EEOnCECreate()
	{
		int rand = Math.RandomInt(0,10);
		float hp = GetMaxHealth()*0.1;
		Print( hp );
		
		if ( rand > 6 )
		{
			ChangeFoodStage( FoodStageType.ROTTEN );
			SetHealth( "", "", hp );
		}
		else if ( rand > 2 )
		{
			ChangeFoodStage( FoodStageType.DRIED );
			SetHealth( "", "", GetMaxHealth()*0.4 );
		}
	}
}
