class ActionSawPlanksCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_CONSTRUCT);
	}
};

class ActionSawPlanks: ActionContinuousBase
{	
	static const int DECREASE_HEALTH_OF_TOOL_DEFAULT = 1; // any other item, including hacksaw
	static const int DECREASE_HEALTH_OF_TOOL_AXE = 2; // axes
	static const int DECREASE_FUEL_OF_CHAINSAW = 3; // chainsaw fuel in ml
	
	void ActionSawPlanks()
	{
		m_CallbackClass = ActionSawPlanksCB;
		m_MessageStartFail = "The tool is ruined.";
		m_MessageStart = "I have started sawing the planks.";
		m_MessageSuccess = "I have sawn 3 planks.";
		m_MessageFail = "I've stopped sawing the planks.";
		m_MessageCancel = "I've stopped sawing the planks.";
		//m_Animation = "startFire";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_MEDIUM;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_SAW_PLANKS;
	}

	override string GetText()
	{
		return "#saw_planks";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_O = target.GetObject();
		
		if ( item  &&  target_O.IsInherited(PileOfWoodenPlanks))
		{
			string item_type = item.GetType();
			
			switch(item_type)
			{
				/*case "Hacksaw": 
					
				break;*/
				
				case "Chainsaw":
					if ( item.HasEnergyManager()  &&  item.GetCompEM().CanWork() )
					{
						return true;
					}
					else
					{
						return false;
					}
				break;
			}
			
			return true;
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		PileOfWoodenPlanks item_POWP = PileOfWoodenPlanks.Cast( action_data.m_Target.GetObject() );
		item_POWP.RemovePlanks(1);
		
		vector pos = action_data.m_Player.GetPosition();
		ItemBase planks = ItemBase.Cast( GetGame().CreateObject("WoodenPlank", pos) );
		const float NEW_PLANKS = 3;

		planks.SetQuantity( Math.Round( action_data.m_Player.GetSoftSkillManager().AddSpecialtyBonus( NEW_PLANKS, this.GetSpecialtyWeight() ) ), true );
		
		ItemBase item = action_data.m_MainItem;
		
		string item_type = item.GetType();
			
			switch(item_type)
			{
				case "WoodAxe": 
					item.DecreaseHealth( "", "", DECREASE_HEALTH_OF_TOOL_AXE);
				break;
			
				case "FirefighterAxe": 
					item.DecreaseHealth( "", "", DECREASE_HEALTH_OF_TOOL_AXE);
				break;
			
				case "FirefighterAxe_Black": 
					item.DecreaseHealth( "", "", DECREASE_HEALTH_OF_TOOL_AXE);
				break;
			
				case "FirefighterAxe_Green": 
					item.DecreaseHealth( "", "", DECREASE_HEALTH_OF_TOOL_AXE);
				break;
			
				case "Hatchet": 
					item.DecreaseHealth( "", "", DECREASE_HEALTH_OF_TOOL_AXE);
				break;
			
				case "Chainsaw":
					if ( item.HasEnergyManager() )
					{
						item.GetCompEM().ConsumeEnergy(DECREASE_FUEL_OF_CHAINSAW);
					}
				break;
			
				default: // Hacksaw and other
					item.DecreaseHealth( "", "", DECREASE_HEALTH_OF_TOOL_DEFAULT);
				break
			}		
		
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};