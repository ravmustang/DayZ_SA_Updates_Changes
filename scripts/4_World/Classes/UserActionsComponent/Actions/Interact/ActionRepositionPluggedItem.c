class ActionRepositionPluggedItem: ActionInteractBase
{
	protected ItemBase m_SourceForReplug = NULL;
	
// Through this action players can reposition already placed electric devices without unplugging them from the power source.
	void ActionRepositionPluggedItem()
	{
		m_MessageStart = "";
		m_MessageSuccess = "I've picked up the device.";
	}

	override int GetType()
	{
		return AT_REPOSITION_PLUGGED_ITEM;
	}

	override string GetText()
	{
		return "Reposition";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase target_IB = ItemBase.Cast( target.GetObject() );
		
		if ( !player.GetItemInHands() || ItemIsBeingRepositioned() )
		{
		if (target_IB.HasEnergyManager())
		{
				if ( target_IB.GetCompEM().IsPlugged() || ItemIsBeingRepositioned() )
				{
					return true;
				}
			}
		}

		return false;
	}

	override void OnStartServer( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );
		EntityAI replug_device = target_IB.GetCompEM().GetEnergySource();
		m_SourceForReplug = ItemBase.Cast(replug_device);
	
		if ( target_IB.IsKindOf("CableReel") )
		{
			CableReel CR = CableReel.Cast( target_IB );
			CR.ForceIntoHandsNow(action_data.m_Player);
		}
		else
		{
			action_data.m_Player.LocalTakeEntityToHands(target_IB);
		}
	}

	override void OnStartClient( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );
		EntityAI replug_device = target_IB.GetCompEM().GetEnergySource();
		m_SourceForReplug = ItemBase.Cast(replug_device);
		
		if ( target_IB.IsKindOf("CableReel") )
		{
			CableReel CR = CableReel.Cast( target_IB );
			CR.ForceIntoHandsNow(action_data.m_Player);
		}
		else
		{
			action_data.m_Player.LocalTakeEntityToHands(target_IB);
		}
	}
	
	override void OnCompleteServer( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();	
		ItemBase target_IB = ItemBase.Cast( targetObject );
		
		if ( m_SourceForReplug.HasEnergyManager() )
		{
			ItemBase attached_device = GetAttachedDevice(m_SourceForReplug);
			
			if (attached_device)
			{
				m_SourceForReplug = attached_device;
			}
			
			target_IB.GetCompEM().PlugThisInto(m_SourceForReplug);
					
			if ( !action_data.m_Player.IsPlacingServer() )
			{
				target_IB.GetInventory().TakeEntityAsAttachment( InventoryMode.LOCAL, m_SourceForReplug );
			}
		}
		
		m_SourceForReplug = NULL;
	}
	
	override void OnCompleteClient( ActionData action_data )
	{	
		if ( !action_data.m_Player.IsPlacingLocal() )
		{
			action_data.m_Player.TogglePlacingLocal();
		}
		
		m_SourceForReplug = NULL;
	}
	
	ItemBase GetAttachedDevice(ItemBase parent)
	{
		string parent_type = parent.GetType();
		
		if ( parent.IsInherited(CarBattery)  ||  parent.IsInherited(TruckBattery) )
		{
			ItemBase parent_attachment = ItemBase.Cast( parent.GetAttachmentByType(MetalWire) );
			
			if (!parent_attachment)
			{
				parent_attachment = ItemBase.Cast( parent.GetAttachmentByType(BarbedWire) );
			}
			return parent_attachment;
		}
		
		return NULL;
	}
	
	ItemBase ItemIsBeingRepositioned()
	{
		return m_SourceForReplug;
	}
};