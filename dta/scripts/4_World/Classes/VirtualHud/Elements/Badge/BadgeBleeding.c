class BadgeBleeding extends DisplayElementBadge 
{
	void BadgeBleeding()
	{
		NUM_OF_BITS = 1;
		m_Key 	= 	NTFKEY_BLEEDISH;
		m_Type	=	eDisplayElements.DELM_BADGE_BLEEDING;
		m_IsClientOnly = true;
	}
}