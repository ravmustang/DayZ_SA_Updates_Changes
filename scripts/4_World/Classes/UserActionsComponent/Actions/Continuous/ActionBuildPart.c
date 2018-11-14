class ActionBuildPartCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_CONSTRUCT);
	}
};

class ActionBuildPart: ActionContinuousBase
{
	void ActionBuildPart()
	{
		m_CallbackClass = ActionBuildPartCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_ASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		
		m_MessageStartFail = "I cannot build a construction part.";
		m_MessageStart = "I have build a construction part.";
		m_MessageSuccess = "I have build a construction part.";
		m_MessageFail = "I have failed to build a construction part.";
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.BASEBUILDING);
	}

	override int GetType()
	{
		return AT_BUILD_PART;
	}
		
	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player )
		{
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			ConstructionPart constrution_part = construction_action_data.GetCurrentBuildPart();
			
			if ( constrution_part )
			{
				return "#build" + " " + constrution_part.GetName();
			}
		}
		
		return "";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
			Construction construction = base_building.GetConstruction();
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			construction_action_data.SetTarget( targetObject );
			
			string main_part_name = targetObject.GetActionComponentName( target.GetComponentIndex() );
			
			//Debug
			//base_building.GetConstruction().IsColliding( part_name );
			
			if ( !GetGame().IsMultiplayer() || GetGame().IsServer() )
			{
				construction_action_data.RefreshPartsToBuild( main_part_name, item );
			}
			ConstructionPart constrution_part = construction_action_data.GetCurrentBuildPart();
			
			if ( constrution_part && base_building.IsFacingBack( player ) )
			{
				return true;
			}			
		}
		
		return false;
	}
		
	override void OnFinishProgressServer( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		ConstructionActionData construction_action_data = action_data.m_Player.GetConstructionActionData();
		string part_name = construction_action_data.GetCurrentBuildPart().GetPartName();
		
		if ( !construction.IsColliding( part_name ) && construction.CanBuildPart( part_name, action_data.m_MainItem ) )
		{
			//build
			construction.BuildPart( part_name, true );
			
			//add damage to tool
			action_data.m_MainItem.DecreaseHealth ( 2 );
		}
		else
		{
			SendMessageToClient( action_data.m_Player, base_building.MESSAGE_CANNOT_BE_CONSTRUCTED );
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	//setup
	override bool SetupAction( PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL )
	{	
		if( super.SetupAction( player, target, item, action_data, extra_data ) )
		{
			SetBuildingAnimation( item );
			
			return true;
		}
		
		return false;
	}
	
	void SetBuildingAnimation( ItemBase item )
	{
		switch( item.Type() )
		{
			case Shovel:
			case FieldShovel:
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGMANIPULATE;
				break;
			case Pliers:
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
				break;				
			default:
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_ASSEMBLE;
				break;
		}
	}
}