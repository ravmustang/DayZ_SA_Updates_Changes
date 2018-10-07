class PPEffects
{
	//-------------------------------------------------------
	// BLUR START
	//-------------------------------------------------------
	static int 		m_BlurInventory;
	static int 		m_BlurDrunk;
	static int		m_BlurFever;
	static int 		m_BlurMenu;
	static int 		m_BlurOptics;
	
	static int	 	m_HitColor;
	static int	 	m_BurlapBlindness;
	
	static float	m_BloodSaturation;
	
	static ref array<float> m_BlurValues;
	static ref map<int, ref array<float>> m_ColorValues;
	static ref array<float> m_ColorEffect;
	
	static float m_UnconsciousVignetteColor[4];
	static float m_UnconsciousVignetteIntesity;
	
	static void Init()
	{
		if ( m_BlurValues )
		{
			delete m_BlurValues;
		}
		
		m_BlurValues = new array<float>;
		
		// add new blur effects here
		m_BlurInventory		= RegisterBlurEffect();
		m_BlurDrunk			= RegisterBlurEffect();
		m_BlurFever			= RegisterBlurEffect();
		m_BlurMenu			= RegisterBlurEffect();
		m_BlurOptics 		= RegisterBlurEffect();
		
		if ( m_ColorEffect )
		{
			delete m_ColorEffect;
		}
		
		m_ColorEffect = new array<float>;
		m_ColorValues = new map<int, ref array<float>>;
		
		// add new color effects here
		m_HitColor 			= RegisterColorEffect();
		m_BurlapBlindness 	= RegisterColorEffect();
	}
	
	static void ResetBlurEffects()
	{	
		if( m_BlurValues )
		{
			for ( int i = 0; i < m_BlurValues.Count(); ++i )
			{
				m_BlurValues[i] = 0;
			}
		}	
	}
		
	static void SetBlurValue(int index, float value)
	{
		if ( m_BlurValues && index < m_BlurValues.Count() )
		{
			m_BlurValues[index] = value;
		}
		else
		{
			Print("Error: PPEffects: m_BlurValues with index: "+ index +" is not registered.");
		}
	}
	
	static int RegisterBlurEffect()
	{
		return m_BlurValues.Insert(0);
	}
	
	//-------------------------------------------------------
	//! sets blur effect to an absolute value between 0..1
	static void SetBlur(float value)
	{
		Material mat_blur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/gauss");
		
		if (mat_blur)
		{
			mat_blur.SetParam("Intensity", value);
		}
	}
	//-------------------------------------------------------
	//! updates the blur post process effect where the resulting blur is an aggregate of all individual blur effect values
	static void UpdateBlur()
	{		
		float blur_value_total = 0;
		if( m_BlurValues )
		{
			for ( int i = 0; i < m_BlurValues.Count(); ++i )
			{
				blur_value_total += m_BlurValues[i];
			}
		}
		
		SetBlur( blur_value_total );
	}
	//-------------------------------------------------------
	//! Set blur inventory effect to a specified 'value' between 0..1
	static void SetBlurInventory(float value)
	{
		SetBlurValue(m_BlurInventory, value);
		UpdateBlur();
	}
	//-------------------------------------------------------
	//! Set blur drunk effect to a specified 'value' between 0..1
	static void SetBlurDrunk(float value)
	{
		SetBlurValue(m_BlurDrunk, value);
		UpdateBlur();
	}

	//-------------------------------------------------------
	//! Set blur drunk effect to a specified 'value' between 0..1
	static void SetBlurFever(float value)
	{
		SetBlurValue(m_BlurFever, value);
		UpdateBlur();
	}
	
	//-------------------------------------------------------
	//! Set menu blur to a specified 'value' between 0..1
	static void SetBlurMenu(float value)
	{
		SetBlurValue(m_BlurMenu, value);
		UpdateBlur();
	}
	
	//-------------------------------------------------------
	//! Set optics and ironsights blur to a specified 'value' between 0..1
	static void SetBlurOptics(float value)
	{
		SetBlurValue(m_BlurOptics, value);
		UpdateBlur();
	}
	
	//-------------------------------------------------------
	// BLUR END
	//-------------------------------------------------------
	/*
	static void SetOverlayColor(float r, float g, float b, float a)
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/glow");
		
		m_Color[0] = r;
		m_Color[1] = g;
		m_Color[2] = b;
		m_Color[3] = a;
		
		matColors.SetParam("OverlayColor", m_Color);
		matColors.SetParam("OverlayFactor", a);

	}*/
	
	static void ResetColorEffects()
	{	
		if( m_ColorEffect )
		{
			for ( int i = 0; i < m_ColorEffect.Count(); ++i )
			{
				m_ColorEffect[i] = 0;
			}
		}
	}
	
	static void SetColorValue(int index, float r, float g, float b, float a, float overlay)
	{
		if ( index < m_ColorEffect.Count() )
		{		
			ref array<float> values = {r,g,b,a,overlay};
			
			m_ColorValues.Set(index, values);
		}
		else
		{
			Print("Error: PPEffects: m_ColorValues with index: "+ index +" is not registered.");
		}
	}
	
	static int RegisterColorEffect()
	{
		return m_ColorEffect.Insert(0);
	}	
	
	static void UpdateColor()
	{
		float color_value_total[4];
		float color_overlay;
	
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/glow");

		for ( int i = 0; i < m_ColorValues.Count(); ++i )
		{
			int key = m_ColorValues.GetKey(i);
			ref array<float> value = m_ColorValues.Get(key);
			
			color_value_total[0] 	= color_value_total[0] + value[0];
			color_value_total[1] 	= color_value_total[1] + value[1];
			color_value_total[2] 	= color_value_total[2] + value[2];
			color_value_total[3] 	= color_value_total[3] + value[3];
			color_overlay  			+= value[4];
		}
				
		matColors.SetParam("OverlayColor", color_value_total);
		matColors.SetParam("OverlayFactor", color_overlay);	
	}

	/*!
	set lens effect 
	\param lens	<-5, 5>, 0 = disable (performance plus), > 0 = outside effect, < 0 inside effect
	\param chromAbb	<0, 1>, chromaticity, 1 = max, 0 disable (performance plus)
	\param centerX	<-1, 1>, center of effect, 0 = screen center in X
	\param centerY	<-1, 1>, center of effect, 0 = screen center in Y
	*/
	static void SetLensEffect(float lens, float chromAbb, float centerX, float centerY)
	{
		Material matHDR = GetGame().GetWorld().GetMaterial("Graphics/Materials/postprocess/glow");
                matHDR.SetParam("LensDistort", lens);
                matHDR.SetParam("MaxChromAbberation", chromAbb);
                matHDR.SetParam("LensCenterX", centerX);
                matHDR.SetParam("LensCenterY", centerY);
	}

	/*!
	set vignette
	\param intensity <0, 1>, intensity of effect, 0 = disable
	\param R	
	\param G	
	\param B	
	*/
	static void SetVignette(float intensity, float R, float G, float B)
	{
		Material matHDR = GetGame().GetWorld().GetMaterial("Graphics/Materials/postprocess/glow");

		float color[4];
		color[0] = R;
		color[1] = G;
		color[2] = B;
		color[3] = 0;

                matHDR.SetParam("Vignette", intensity);
                matHDR.SetParam("VignetteColor", color);
	}


	static void ResetVignette()
	{
		SetVignette(0,0,0,0);
	}

	static void OverrideDOF(bool enable, float focusDistance, float focusLength, float focusLengthNear, float blur, float focusDepthOffset)
	{
		GetGame().OverrideDOF(enable, focusDistance, focusLength, focusLengthNear, blur, focusDepthOffset);
	}

	static void AddPPMask(float ndcX, float ndcY, float ndcRadius, float ndcBlur)
	{
		GetGame().AddPPMask(ndcX, ndcY, ndcRadius, ndcBlur);
	}

	static void ResetPPMask()
	{
		GetGame().ResetPPMask();
	}
	
	static void ResetDOFOverride()
	{
		OverrideDOF(false,0,0,0,0,0);
	}
	
	static void ResetLensEffect()
	{
		SetLensEffect(0,0,0,0);
	}

	static void HitEffect(float overlay)
	{
		SetColorValue(m_HitColor, 1, 0, 0, 1, overlay);
		UpdateColor();
	}
	
	static void EnableBurlapSackBlindness()
	{
		SetColorValue(m_BurlapBlindness, 0, 0, 0, 1, 1.0);
		UpdateColor();
		GetGame().SetEVUser(-5);
	}

	static void DisableBurlapSackBlindness()
	{		
		SetColorValue(m_BurlapBlindness, 0, 0, 0, 0, 0.0);
		UpdateColor();
		GetGame().SetEVUser(0);
	}

	static void UpdateSaturation()
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/glow");
		matColors.SetParam("Saturation", m_BloodSaturation/*+add_additional_modifiers_here*/);
	}
	
	static void UpdateVignette()
	{
		float color[4];
		float intesity;
		
		color[0] = m_UnconsciousVignetteColor[0]/*+add_additional_modifiers_here*/;
		color[1] = m_UnconsciousVignetteColor[1]/*+add_additional_modifiers_here*/;
		color[2] = m_UnconsciousVignetteColor[2]/*+add_additional_modifiers_here*/;
		
		intesity = m_UnconsciousVignetteIntesity/*+add_additional_modifiers_here*/;
		
		SetVignette( intesity, color[0], color[1], color[2] );
		
	}

	static void SetBloodSaturation(float value)
	{
		m_BloodSaturation = value;
		UpdateSaturation();
	}
	
	static void SetUnconsciousnessVignette(float value)
	{
		m_UnconsciousVignetteIntesity = value;
		UpdateVignette();
	}
	
	static void RemoveUnconsciousnessVignette()
	{
		m_UnconsciousVignetteIntesity = 0;
		UpdateVignette();
	}
	
	static void ResetAll()
	{		
		ResetBlurEffects();
		ResetColorEffects();
		ResetVignette();
		ResetPPMask();
		ResetDOFOverride();
		ResetLensEffect();
		SetBloodSaturation(1);
	}	
};