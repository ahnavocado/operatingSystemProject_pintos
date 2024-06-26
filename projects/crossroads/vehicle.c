
#include <stdio.h>

#include "threads/thread.h"
#include "threads/synch.h"
#include "projects/crossroads/vehicle.h"
#include "projects/crossroads/map.h"
#include "projects/crossroads/ats.h"

/* path. A:0 B:1 C:2 D:3 */
const struct position vehicle_path[4][4][12] = {
	/* from A */ {
		/* to A */
		{
			{4, 0},
			{4, 1},
			{4, 2},
			{4, 3},
			{4, 4},
			{3, 4},
			{2, 4},
			{2, 3},
			{2, 2},
			{2, 1},
			{2, 0},
			{-1, -1},
		},
		/* to B */
		{
			{4, 0},
			{4, 1},
			{4, 2},
			{5, 2},
			{6, 2},
			{-1, -1},
		},
		/* to C */
		{
			{4, 0},
			{4, 1},
			{4, 2},
			{4, 3},
			{4, 4},
			{4, 5},
			{4, 6},
			{-1, -1},
		},
		/* to D */
		{
			{4, 0},
			{4, 1},
			{4, 2},
			{4, 3},
			{4, 4},
			{3, 4},
			{2, 4},
			{1, 4},
			{0, 4},
			{-1, -1},
		}},
	/* from B */ {/* to A */
				  {
					  {6, 4},
					  {5, 4},
					  {4, 4},
					  {3, 4},
					  {2, 4},
					  {2, 3},
					  {2, 2},
					  {2, 1},
					  {2, 0},
					  {-1, -1},
				  },
				  /* to B */
				  {
					  {6, 4},
					  {5, 4},
					  {4, 4},
					  {3, 4},
					  {2, 4},
					  {2, 3},
					  {2, 2},
					  {3, 2},
					  {4, 2},
					  {5, 2},
					  {6, 2},
					  {-1, -1},
				  },
				  /* to C */
				  {
					  {6, 4},
					  {5, 4},
					  {4, 4},
					  {4, 5},
					  {4, 6},
					  {-1, -1},
				  },
				  /* to D */
				  {
					  {6, 4},
					  {5, 4},
					  {4, 4},
					  {3, 4},
					  {2, 4},
					  {1, 4},
					  {0, 4},
					  {-1, -1},
				  }},
	/* from C */ {/* to A */
				  {
					  {2, 6},
					  {2, 5},
					  {2, 4},
					  {2, 3},
					  {2, 2},
					  {2, 1},
					  {2, 0},
					  {-1, -1},
				  },
				  /* to B */
				  {
					  {2, 6},
					  {2, 5},
					  {2, 4},
					  {2, 3},
					  {2, 2},
					  {3, 2},
					  {4, 2},
					  {5, 2},
					  {6, 2},
					  {-1, -1},
				  },
				  /* to C */
				  {
					  {2, 6},
					  {2, 5},
					  {2, 4},
					  {2, 3},
					  {2, 2},
					  {3, 2},
					  {4, 2},
					  {4, 3},
					  {4, 4},
					  {4, 5},
					  {4, 6},
					  {-1, -1},
				  },
				  /* to D */
				  {
					  {2, 6},
					  {2, 5},
					  {2, 4},
					  {1, 4},
					  {0, 4},
					  {-1, -1},
				  }},
	/* from D */ {/* to A */
				  {
					  {0, 2},
					  {1, 2},
					  {2, 2},
					  {2, 1},
					  {2, 0},
					  {-1, -1},
				  },
				  /* to B */
				  {
					  {0, 2},
					  {1, 2},
					  {2, 2},
					  {3, 2},
					  {4, 2},
					  {5, 2},
					  {6, 2},
					  {-1, -1},
				  },
				  /* to C */
				  {
					  {0, 2},
					  {1, 2},
					  {2, 2},
					  {3, 2},
					  {4, 2},
					  {4, 3},
					  {4, 4},
					  {4, 5},
					  {4, 6},
					  {-1, -1},
				  },
				  /* to D */
				  {
					  {0, 2},
					  {1, 2},
					  {2, 2},
					  {3, 2},
					  {4, 2},
					  {4, 3},
					  {4, 4},
					  {3, 4},
					  {2, 4},
					  {1, 4},
					  {0, 4},
					  {-1, -1},
				  }}};
int vehicle_cnt;
float step_cnt=0.0;
int checkVehicleInCrossroad()
{
	int vehicleInCroosroad = 0;
	struct vehicle_info *vi;

	for (int i = 2; i <= 4; i++)
	{
		for (int j = 2; j <= 4; j++)
		{
			printf("%d-----%d\n", i, j);
			// if(lock_try_acquire(&vi->map_locks[i][j])){
			// 	vehicleInCroosroad++;

			// }
			// printf("---------%B\n",lock_try_acquire(&vi->map_locks[i][j]));
		}
	}
	// printf(vehicleInCroosroad);
	return vehicleInCroosroad;
}

static int is_position_outside(struct position pos)
{
	return (pos.row == -1 || pos.col == -1);
}

/* return 0:termination, 1:success, -1:fail */
static int try_move(int start, int dest, int step, struct vehicle_info *vi)
{
	struct position pos_cur, pos_next;
	int vehicleInCroosroad = 0;

	pos_next = vehicle_path[start][dest][step];
	pos_cur = vi->position;

	if (vi->state == VEHICLE_STATUS_RUNNING)
	{
		/* check termination */
		if (is_position_outside(pos_next))
		{
			/* actual move */
			vi->position.row = vi->position.col = -1;
			/* release previous */
			lock_release(&vi->map_locks[pos_cur.row][pos_cur.col]);
			return 0;
		}
	}
	vehicleInCroosroad = 0;
	if((vi->start=='A'&&vi->position.col==1&&vi->position.row==4)||(vi->start=='B'&&vi->position.col==4&&vi->position.row==5)||(vi->start=='C'&&vi->position.col==5&&vi->position.row==2)||(vi->start=='D'&&vi->position.col==2&&vi->position.row==1)){
	for (int i = 2; i <= 4; i++){
		for (int j = 2; j <= 4; j++){
			if (i == vi->position.row && j == vi->position.col){
			}
			else
			{
				if (lock_try_acquire(&vi->map_locks[i][j])){
					lock_release(&vi->map_locks[i][j]);
				}
				else{
					vehicleInCroosroad++;
				}
			}
		}
	}
	if (vehicleInCroosroad >= 2)
	{
		return -1;
	}
	}
	// printf("%d||||||||||%d\n",vi->state,vehicleInCroosroad);
	
	
	
	/* lock next position */
	lock_acquire(&vi->map_locks[pos_next.row][pos_next.col]);
	if (vi->state == VEHICLE_STATUS_READY)
	{
		/* start this vehicle */
		vi->state = VEHICLE_STATUS_RUNNING;
	}
	else
	{
		/* release current position */
		lock_release(&vi->map_locks[pos_cur.row][pos_cur.col]);
	}
	/* update position */
	vi->position = pos_next;

	return 1;
	
}

void init_on_mainthread(int thread_cnt)
{
	vehicle_cnt = thread_cnt;
	/* Called once before spawning threads */
}

void vehicle_loop(void *_vi)
{
	int res;
	int start, dest, step;

	struct vehicle_info *vi = _vi;

	start = vi->start - 'A';
	dest = vi->dest - 'A';

	vi->position.row = vi->position.col = -1;
	vi->state = VEHICLE_STATUS_READY;

	step = 0;
	while (1)
	{
		/* vehicle main code */
		res = try_move(start, dest, step, vi);
		if (res == 1)
		{
			step++;
			
		}

		/* termination condition. */
		if (res == 0)
		{
			break;
		}
		if (res == -1)
		{
			continue;
		}
		
		// step_cnt=step_cnt+(8/vehicle_cnt);
		// step_cnt=step_cnt+vehicle_cnt;
		// print("--------%d",step_cnt);
		crossroads_step++;

		/* unitstep change! */
		unitstep_changed();
	}

	/* status transition must happen before sema_up */
	vi->state = VEHICLE_STATUS_FINISHED;
}
