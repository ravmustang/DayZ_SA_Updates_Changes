class ActionDeconstructCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DECONSTRUCT);
	}
};

class ActionDeconstruct: ActionContinuousBase
{
	float m_DamageAmount;
	string m_SlotName;
	
	void ActionDeconstruct()
	{
		m_CallbackClass = ActionDeconstructCB;
		m_MessageStartFail = "I cannot build a construction part.";
		m_MessageStart = "I have build a construction part.";
		m_MessageSuccess = "I have build a construction part.";
		m_MessageFail = "I have failed to build a construction part.";
		m_DamageAmount = 2;
		//m_Animation = "startFire";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_DECONSTRUCT;
	}
		
	override string GetText()
	{
		return "Deconstruct";		//add name of the construction part if possible
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( target.GetObject() );
		if ( base_building )
		{
			string slot_name = base_building.GetActionComponentName(target.GetComponentIndex());
			
			Construction construction = base_building.GetConstruction();
			
			if ( construction.CanDeconstructPart( slot_name ) )
			{
				//debug
				//action_data.m_Player.MessageAction( slot_name );
				
				return true;
			}
		}
		
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		
		//Get actual construcion slot
		string slot_name = targetObject.GetActionComponentName(action_data.m_Target.GetComponentIndex());
		SetSlotName( slot_name );
	}
	
	override void OnStartClient( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		
		//Get actual construcion slot
		string slot_name = targetObject.GetActionComponentName(action_data.m_Target.GetComponentIndex());
		SetSlotName( slot_name );
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		string part_name = construction.GetDeconstructionPartName( GetSlotName() );
		
		if ( !construction.HasDependentPart( part_name ) )
		{
			//construct
			construction.DeconstructPart( GetSlotName(), part_name );
			
			//add damage to tool
			action_data.m_MainItem.DecreaseHealth ( "", "", m_DamageAmount, true );
		}
		else
		{
			SendMessageToClient( action_data.m_Player, base_building.MESSAGE_CANNOT_BE_DECONSTRUCTED );
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	protected void SetSlotName( string slot_name )
	{
		m_SlotName = slot_name;
	}
	protected string GetSlotName()
	{
		return m_SlotName;
	}
}