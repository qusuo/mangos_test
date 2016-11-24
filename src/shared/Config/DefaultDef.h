#ifndef __DEFAULT_DEF_H
#define __DEFAULT_DEF_H


#define CONTACT_DISTANCE            0.5f
#define INTERACTION_DISTANCE        5.0f
#define ATTACK_DISTANCE             5.0f
#define INSPECT_DISTANCE            11.11f
#define TRADE_DISTANCE              11.11f
#define MAX_VISIBILITY_DISTANCE     333.0f      // max distance for visible object show, limited in 333 yards
#define DEFAULT_VISIBILITY_DISTANCE 90.0f       // default visible distance, 90 yards on continents
#define DEFAULT_VISIBILITY_INSTANCE 120.0f      // default visible distance in instances, 120 yards
#define DEFAULT_VISIBILITY_BG       180.0f      // default visible distance in BG, 180 yards

#define DEFAULT_WORLD_OBJECT_SIZE   0.388999998569489f      // currently used (correctly?) for any non Unit world objects. This is actually the bounding_radius, like player/creature from creature_model_data
#define DEFAULT_OBJECT_SCALE        1.0f                    // non-Tauren player/item scale as default, npc/go from database, pets from dbc
#define DEFAULT_TAUREN_MALE_SCALE   1.35f                   // Tauren male player scale by default
#define DEFAULT_TAUREN_FEMALE_SCALE 1.25f                   // Tauren female player scale by default

#define MAX_STEALTH_DETECT_RANGE    45.0f

#define DEFAULT_PACK_BUF 200

#endif
