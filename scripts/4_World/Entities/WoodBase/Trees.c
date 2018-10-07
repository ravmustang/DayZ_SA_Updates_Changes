class TreeHard_t_BetulaPendula_1f: TreeHard 
{
	override void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( item && item.KindOf("Knife") )
		{
			output_map.Insert("Bark_Birch",1);
		}
		else if ( item && item.KindOf("Axe") )
		{
			output_map.Insert("FireWood",1);
			output_map.Insert("WoodenStick",1);
		}
	}
};

class TreeHard_t_BetulaPendula_1fb: TreeHard_t_BetulaPendula_1f
{              
};             
               
class TreeHard_t_BetulaPendula_1s: TreeHard_t_BetulaPendula_1f
{              
};             
class TreeHard_t_BetulaPendula_2f: TreeHard_t_BetulaPendula_1f
{              
};             
               
class TreeHard_t_BetulaPendula_2fb: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_2fc: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_2s: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_3f: TreeHard_t_BetulaPendula_1f
{                
};               
                 
class TreeHard_t_BetulaPendula_3fb: TreeHard_t_BetulaPendula_1f
{              
};             
               
class TreeHard_t_BetulaPendula_3s: TreeHard_t_BetulaPendula_1f
{               
};              
                
class TreeHard_t_BetulaPendula_3fc: TreeHard_t_BetulaPendula_1f
{
}; 

class TreeSoft_t_malus1s: TreeSoft
{
};