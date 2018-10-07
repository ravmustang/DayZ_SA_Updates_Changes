//WIP
class SEffectManager
{
	protected static ref array<int> m_FreeEffectIDs = new array<int>;
	protected static ref map<int, ref Effect> m_EffectsMap = new map<int, ref Effect>;
	protected static int m_HighestFreeEffectID = 1;
	
	
	static private int EffectRegister(Effect eff)
	{
		int key = GetFreeEffectID();
		m_EffectsMap.Insert(key, eff);
		eff.SetID(key);
		
		return key;
	}
	
	static int GetFreeEffectID()
	{
		int return_id;
		
		if (m_FreeEffectIDs.Count() > 0)
		{
			return_id = m_FreeEffectIDs.Get(0);
			m_FreeEffectIDs.Remove(0);
		}
		else
		{
			return_id = m_HighestFreeEffectID;
			m_HighestFreeEffectID++; //As of 12.4.2018 this trick doesn't work: return_id = m_HighestFreeEffectID++;
		}
		
		return return_id;
		
	}
	
	static int PlayInWorld(Effect eff, vector pos, vector ori = "0 0 0")
	{
		eff.SetPosition( pos );
		eff.SetOrientation( ori );
		
		eff.Play();
		int id = EffectRegister(eff);
		
		return id;
	}
	
	static int PlayOnObject(Effect eff, Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		eff.Play();
		eff.AttachTo( obj, local_pos, local_ori);
		int id = EffectRegister(eff);
		
		return id;
	}
	
	static bool IsExist( int effect_id )
	{
		if ( m_EffectsMap[effect_id] )
		{
			return true;
		}
		
		return false;
	}
	
	static void Stop(int effect_id)
	{
		Effect eff = m_EffectsMap.Get(effect_id);
		
		if (eff)
		{
			eff.Stop();
		}
		else
		{
			string str = "SEffectManager> Stopping an effect with ID " + effect_id + " has failed! Such ID is not registered in m_EffectsMap!";
			Error(str);
		}
	}
	
	static Effect GetEffectByID(int effect_id)
	{
		return m_EffectsMap.Get(effect_id);
	}
	
	static void EffectUnregister(int id)
    {
		m_EffectsMap.Set(id, NULL);
		m_FreeEffectIDs.Insert(id);
    }
	
	
	/*static array<ref Effect> GetAllEffects()
	{
		return m_Effects;
	}*/
}