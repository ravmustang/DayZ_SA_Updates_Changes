class ActionFillObjectCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_FILL);
	}
};

class ActionFillObject: ActionContinuousBase
{
	protected int m_ActionState;

	protected const int EMPTY 						= 0;
	protected const int FILLED 						= 1;
	
	void ActionFillObject()
	{
		m_CallbackClass = ActionFillObjectCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		/*
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		m_MessageStartFail = "Failed to start action: Object collides with environment.";
		*/
		//m_Animation = "DIGGINGSHOVEL";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_FILL_OBJECT;
	}
		
	override string GetText()
	{
		if( m_ActionState == FILLED )
		{
			return "Empty the object";
		}
		else
		{
			return "Fill the object";
		}
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HescoBox hesco;
		if ( Class.CastTo(hesco,target.GetObject()) )
		{
			if ( hesco.CanBeFilledAtPosition( player.GetPosition() ) )
			{
				if ( hesco.GetState() == HescoBox.UNFOLDED )
				{
					m_ActionState = EMPTY;
					return true;
				}
				else if( hesco.GetState() == HescoBox.FILLED)
				{
					m_ActionState = FILLED;
					return true;					
				}
			}
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		HescoBox hesco;
		if ( Class.CastTo(hesco,action_data.m_Target.GetObject()) )
		{
			const float ITEM_DAMAGE = 0.05;
			action_data.m_MainItem.DecreaseHealth ( "", "", action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() )*100 );
			
			if ( hesco.GetState() == HescoBox.UNFOLDED )
			{
				hesco.Fill();
			}
			else if ( hesco.GetState() == HescoBox.FILLED )
			{
				hesco.Unfold();
			}
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};