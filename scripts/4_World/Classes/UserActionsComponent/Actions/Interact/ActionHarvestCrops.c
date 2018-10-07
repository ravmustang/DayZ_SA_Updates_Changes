class ActionHarvestCrops: ActionInteractBase
{
	void ActionHarvestCrops()
	{
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";

		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
	}

	override int GetType()
	{
		return AT_HARVEST_CROPS;
	}

	override string GetText()
	{
		return "Harvest crops";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject.IsInherited(PlantBase) )
		{
			PlantBase plant = PlantBase.Cast( targetObject );
			
			if (  ( plant.IsMature()  ||  plant.IsSpoiled() )  &&  plant.HasCrops()  )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		PlantBase plant = PlantBase.Cast( targetObject );
		m_MessageSuccess = plant.Harvest( action_data.m_Player );

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};