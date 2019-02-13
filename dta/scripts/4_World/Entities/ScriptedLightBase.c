/*
Lights work only on client side!

TO DO:
- Solve issue with lights glowing when their parent is inside cargo.
- Parent of a light should have emissive glow activated.
- Function for attaching light source on object's memory point, and rotating it towards another memory point.
- Function that disables shadows when the light point gets very close to camera. Major optimization for headtorch, flashlight and torches.

*/

class ScriptedLightBase extends EntityLightSource
{
	private float m_LifetimeStart;
	private float m_LifetimeEnd;
	private float m_FadeOutTime;
	private float m_FadeInTime;
	private float m_DefaultRadius;
	private float m_DefaultBrightness;
	
	private Object m_Parent; // Attachment parent
	private vector m_LocalPos; // Local position to my attachment parent
	private vector m_LocalOri; // Local orientation to my attachment parent
	
	//! Constructor. Everything here is executed before the constructor of all children.
	void ScriptedLightBase()
	{
		m_LifetimeEnd = -1; // -1 makes this light permanent
		m_LifetimeStart = GetGame().GetTime();
		m_FadeOutTime = -1;
		m_FadeInTime = -1;
		SetEnabled(true);
		SetEventMask(EntityEvent.FRAME);
	}
	
	//! Attaches this light on the parent entity, with optional position and orientation offset.
	void AttachOn(Object parent, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		m_Parent = parent;
		m_LocalPos = local_pos;
		m_LocalOri = local_ori;
		SetOrientation(local_ori);
		SetPosition(local_pos);
		parent.AddChild(this, -1);
	}
	
	//! Attaches this light on the parent entity, with optional position and orientation offset. TO DO!
	void AttachOnMemoryPoint(Object parent, string memory_point_location, string memory_point_target)
	{
		/*m_Parent = parent;
		
		m_LocalPos = GetMemoryPointPos(memory_point_location);
		m_LocalOri = local_ori;*/
	}
	
	//! Detaches this light from its parent entity.
	void DetachFromParent()
	{
		m_Parent = null;
		m_LocalPos = Vector(0,0,0);
		m_LocalOri = Vector(0,0,0);
	}
	
	//! Creates an instance of light on the given position. Optionally, use fade_in_time_in_ms parameter to make the light fade into existence.
	static ScriptedLightBase CreateLight(typename name, vector global_pos, float fade_in_time_in_ms = 0)
	{
		ScriptedLightBase light_instance;
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			light_instance = ScriptedLightBase.Cast( GetGame().CreateObject(name.ToString(), global_pos, true) );
			
			if (!light_instance)
			{
				Error("Error! Light entity of name " + name.ToString() + " cannot be spawned! This name is incorrect, or not inherited from ScriptedLightBase." );
				return null;
			}
			
			if (fade_in_time_in_ms != 0)
				light_instance.FadeIn(fade_in_time_in_ms);
			;
		}
		else // Server side
		{
			Error("An instance of ScriptedLightBase was attempted to spawn on server side! Lights are must be CLIENT SIDE ONLY!");
		}
		
		return light_instance;
	}
	
	//! Sets the radius of the light
	void SetRadiusClient(float pValue)
	{
		m_DefaultRadius = pValue;
		SetRadius(m_DefaultRadius);
	}
	
	//! Sets the brightness of the light
	void SetBrightnessClient(float pValue)
	{
		m_DefaultBrightness = pValue;
		SetBrightness(m_DefaultBrightness);
	}
	
	//! Switches off the light and deletes it from memory
	void Destroy()
	{
		ClearEventMask(EntityEvent.FRAME);
		SetEnabled(false);
		GetGame().ObjectDelete(this);
	}
	
	//! Makes the light destroy itself after the given time in miliseconds. 
	void SetLifetime(float life_in_ms)
	{
		m_LifetimeEnd = GetGame().GetTime() + life_in_ms;
	}
	
	//! Sets the fade out time in ms. Fade out begins automatically as the light nears the end of its life time.
	void SetFadeOutTime(float time_in_ms)
	{
		m_FadeOutTime = time_in_ms;
		m_FadeInTime = -1; // Stop fade in process
	}
	
	//! Starts the fade out process and destroys the light when its done. Optional parameter allows you to set time of this fade out. If not set, then default value is used.
	void FadeOut(float time_in_ms = -1)
	{
		if (time_in_ms == -1)
			time_in_ms = m_FadeOutTime;
		else
			m_FadeOutTime = time_in_ms;
		
		m_FadeInTime = -1; // Stop fade in process
		
		SetLifetime(time_in_ms);
	}
	
	//! Makes the light fade into existence. Works only at the moment the light is created.
	void FadeIn(float time_in_ms)
	{
		m_FadeInTime = time_in_ms;
	}
	
	//! Prolongs the lifetime of the light in ms. Use negative number to shorten its lifetime.
	void AddLifetime(float life_in_ms)
	{
		m_LifetimeEnd += life_in_ms;
	}
	
	//! override this for custom functionality
	void OnFrameLightSource(IEntity other, float timeSlice)
	{
		// ...
	}
	
	// On frame event. If you want to control your light within your own rules then override the above event OnFrameLightSource and put your code there.
	override void EOnFrame(IEntity other, float timeSlice)
	{
		// Control lifetime of the light
		float current_time = GetGame().GetTime();
		if ( current_time > m_LifetimeEnd  &&  m_LifetimeEnd != -1 )
		{
			Destroy();
			return;
		}
		else
		{
			SetBrightness(m_DefaultBrightness);
			SetRadius(m_DefaultRadius);
		}
		
		// Control fade out of the light
		if ( m_FadeOutTime != -1  &&  current_time > m_LifetimeEnd - m_FadeOutTime )
		{
			float time_left = m_LifetimeEnd - current_time;
			float intensity_coef = (time_left) / m_FadeOutTime;
			float new_brightness = m_DefaultBrightness * intensity_coef;
			float new_radius = m_DefaultRadius * intensity_coef;
			
			if (new_brightness > 0  &&  new_radius > 0)
			{
				SetBrightness(new_brightness);
				SetRadius(new_radius);
			}
			else
			{
				Destroy();
				return;
			}
		}
		
		// Control fade in of the light
		if ( m_FadeInTime != -1  &&  current_time < m_LifetimeStart + m_FadeInTime )
		{
			float age = current_time - m_LifetimeStart;
			float intensity_coef2 = age / m_FadeInTime;
			float new_brightness2 = m_DefaultBrightness * intensity_coef2;
			float new_radius2 = m_DefaultRadius * intensity_coef2;
			
			SetBrightness(new_brightness2);
			SetRadius(new_radius2);
		}
		
		OnFrameLightSource(other, timeSlice);
	}
};