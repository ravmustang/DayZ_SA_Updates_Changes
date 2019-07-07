class FirearmActionUnjam : FirearmActionBase
{	
	void FirearmActionUnjam() 
	{
	}	
	
	override bool HasTarget()
	{
		return false;
	}
	
	override typename GetInputType()
	{
		return ContinuousWeaponManipulationActionInput;
	} 
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProgress()
	{
		return false;
	}
	
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		Weapon_Base wpn = Weapon_Base.Cast(item);
		if (wpn && wpn.CanProcessWeaponEvents())
		{
			if (player.GetWeaponManager().CanUnjam(wpn))
				return true;
		}
		return false;
	}
	
	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.GetWeaponManager().Unjam( this );
	}
};