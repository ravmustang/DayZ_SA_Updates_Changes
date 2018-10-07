class ActionSortAmmoPileCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_SORT);
	}
};

class ActionSortAmmoPile: ActionContinuousBase
{
	void ActionSortAmmoPile()
	{
		m_CallbackClass = ActionSortAmmoPileCB;
		m_MessageStartFail = "It's too torn.";
		m_MessageStart = "I have started putting sack on";
		m_MessageSuccess = "I have put sack on.";
		m_MessageFail = "I have moved and putting sack on was canceled.";
		m_MessageCancel = "I stopped putting sack on.";
		//m_Animation = "INJECTEPIPENS";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_SORT_AMMO;
	}
		
	override string GetText()
	{
		return "Sort ammunition";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return false;
	}

	override void OnCompleteLoopServer( ActionData action_data )
	{	
		SortAmmo(action_data.m_MainItem);
	}

	void SortAmmo(ItemBase item)
	{
		Magazine magazine;
		Magazine piles[5];
		//string pile_classname = magazine.ConfigGetString("spawnPileType");
		if( Class.CastTo(magazine, item) )
		{
			float health;
			string ammo_type;
			while( magazine.GetAmmoCount() > 0)
			{
				int count = magazine.GetAmmoCount();
				if(magazine.ServerAcquireCartridge(health, ammo_type))
				{
					int health_label = MiscGameplayFunctions.GetHealthLabelForAmmo(ammo_type,health);
					if( piles[health_label] == null )
					{
						piles[health_label] = Magazine.Cast( GetGame().CreateObject( item.ClassName(), item.GetPosition() ) );		
					}
					piles[health_label].ServerStoreCartridge(health,ammo_type);
				}
			}
		}
	}
};