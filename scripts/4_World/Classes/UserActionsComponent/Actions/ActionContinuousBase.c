class ActionContinuousBaseCB : ActionBaseCB
{	
	bool m_inLoop = false;
	
	bool CancelCondition()
	{
		//SetCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP);
		//Print("cancel condition enabled: " + GetState().ToString() );
		if ( !m_Interrupted && (GetState() == STATE_LOOP_LOOP || GetState() == STATE_LOOP_LOOP2 || m_inLoop) )
		{	
			if( m_ActionData.m_State == UA_INITIALIZE )
				m_ActionData.m_State = UA_PROCESSING;
			
			
			AnimatedActionBase action = AnimatedActionBase.Cast(m_ActionData.m_Action);
			action.Do(m_ActionData, m_ActionData.m_State);
		}
			return DefaultCancelCondition(); 
	}
	
	
	override void OnAnimationEvent(int pEventID)	
	{
		AnimatedActionBase action = AnimatedActionBase.Cast(m_ActionData.m_Action);
#ifdef DEVELOPER
		Print("ActionInteractBase.c | OnAnimationEvent | OnAnimationEvent called");
#endif
		if ( !m_Interrupted && pEventID == UA_IN_START ) 
		{
			m_inLoop = true;
		}else if ( !m_Interrupted && pEventID == UA_IN_END ) 
		{
			m_inLoop = false;
		}else if ( !m_Interrupted && pEventID == UA_ANIM_EVENT ) 
		{
			action.OnAnimationEvent( m_ActionData );
		}
	}
	
	override void InitActionComponent()
	{
		m_Interrupted = false;
		m_Canceled = false;
		m_inLoop = false;

		CreateActionComponent();
		if ( m_ActionData.m_ActionComponent ) 
		{
			m_ActionData.m_ActionComponent.Init(m_ActionData);		
		}
		m_ActionData.m_State = UA_INITIALIZE;
		RegisterAnimationEvent("ActionExecStart", UA_IN_START);
		RegisterAnimationEvent("ActionExecEnd", UA_IN_END);
		RegisterAnimationEvent("ActionExec", UA_ANIM_EVENT);
		EnableCancelCondition(true);
		m_SoundObject = m_ActionData.m_Action.PlayActionSound(m_ActionData.m_Player);
	}
	
	override void EndActionComponent()
	{
		// TODO for second type animation SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
		if ( m_ActionData.m_State == UA_FINISHED )
		{
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		}
		else if (m_ActionData.m_State == UA_CANCEL )
		{
			m_Canceled = true;
 			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			return;
			//Cancel();
		}
		else
		{
			m_Canceled = true;
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			return;
		}
		m_ActionData.m_State = UA_FINISHED;
	}	
	
	void UserEndsAction()
	{
		if ( m_ActionData.m_ActionComponent ) 
		{
			m_ActionData.m_State = m_ActionData.m_ActionComponent.Cancel(m_ActionData);
		}
		EndActionComponent();
	}
};

class ActionContinuousBase : AnimatedActionBase
{
	void ActionContinuousBase() 
	{
		m_CallbackClass = ActionContinuousBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
	}
	
	override void OnContinuousCancel(ActionData action_data)
	{
		ActionContinuousBaseCB callback;
		if( Class.CastTo(callback, action_data.m_Callback) )
		{
			if ( callback.GetState() != callback.STATE_LOOP_END && callback.GetState() != callback.STATE_LOOP_END2 )
			{
				callback.UserEndsAction();
			}
		}		
	}
	
	override int GetActionCategory()
	{
		return AC_CONTINUOUS;
	}
};