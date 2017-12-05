public class Entity1 extends Entity
{
    // Perform any necessary initialization in the constructor
	int shortestPath[];

	public Entity1()
  {
  		this.shortestPath = new int[4];

			// For each node assign the values in the distanceTable
    	for(int i = 0; i < 4; i++)
			{
    		for(int j = 0; j < 4; j++)
				{
    			if(i == j)
					{
    				switch(i)
						{
    				// Assigns value for node 0 via 0
    				case 0:
    					this.distanceTable[i][j] = 1;
    					break;

						// Assigns value for node 1 via 0
    				case 1:
    					this.distanceTable[i][j] = 0;
    					break;

						// Assigns value for node 2 via 0
    				case 2:
    					this.distanceTable[i][j] = 1;
    					break;

						// Assigns value for node 3 via 0
    				case 3:
    					this.distanceTable[i][j] = 999;
    					break;
    				}
    			}
    			else
					{
							// Assigns every other value to 999 i.e. unknown
        			this.distanceTable[i][j] = 999;
    			}
    		}
    	}

			// Builds the shortestPath array
    	this.GetShortestPath();

			// Prints out the current distanceTable
    	this.printDT();

			// Builds packets to send out to each node
			// Packet(source, destination, shortestPath array)
	    Packet packet1 = new Packet(1, 0, this.shortestPath);
			Packet packet2 = new Packet(1, 2, this.shortestPath);

			// Sends each packet to the specified node i.e. Packet#
			NetworkSimulator.toLayer2(packet1);
			NetworkSimulator.toLayer2(packet2);

    }

		public void GetShortestPath()
		{
				int min;

				// Gets the values of the min of each row
				for(int i = 0; i < 4; i++)
				{
					// Sets the min to 999 i.e. unknown
					min = 999;

					// Iterates through the column and gets the min value
					for(int j = 0; j < 4; j++)
					{
						// Grabs the minimum distance from the current row i.e. 'i'
						if(this.distanceTable[i][j] < min)
						{
							min = this.distanceTable[i][j];
						}
					}

					// Assigns shortest path for each row
					this.shortestPath[i] = min;
				}
		}

    // Handle updates when a packet is received.  Students will need to call
    // NetworkSimulator.toLayer2() with new packets based upon what they
    // send to update.  Be careful to construct the source and destination of
    // the packet correctly.  Read the warning in NetworkSimulator.java for more
    // details.
		public void update(Packet p)
    {
				// Variable to tell if values were changed
    		boolean changed = false;

				// Get node source i.e. node number
    		int source = p.getSource();

    		// Iterate through recieved packets min array added to the Mincost()
				// Received from the node communicating with this current node
    		for(int i = 0; i < 4; i++)
				{
    			if(this.shortestPath[source] + p.getMincost(i) < this.distanceTable[i][source])
					{
    				this.distanceTable[i][source] = this.shortestPath[source] + p.getMincost(i);
    				changed = true;
    			}
    		}

				this.printDT();

				// If there was a change send an update to all other nodes
				if(changed == true)
				{

					// Get the new shortestPath's with newly updated values
					this.GetShortestPath();

					// Builds packets to send out to each node
					// Packet(source, destination, shortestPath array)
					Packet packet1 = new Packet(1, 0, this.shortestPath);
					Packet packet2 = new Packet(1, 2, this.shortestPath);

					// Sends each packet to the specified node i.e. Packet#
					NetworkSimulator.toLayer2(packet1);
					NetworkSimulator.toLayer2(packet2);

			}
		}

		// Graduate Student Stuff
    public void linkCostChangeHandler(int whichLink, int newCost)
    {
    }

    public void printDT()
    {
        System.out.println();
        System.out.println("         via");
        System.out.println(" D1 |   0   2");
        System.out.println("----+--------");
        for (int i = 0; i < NetworkSimulator.NUMENTITIES; i++)
        {
            if (i == 1)
            {
                continue;
            }

            System.out.print("   " + i + "|");
            for (int j = 0; j < NetworkSimulator.NUMENTITIES; j += 2)
            {

                if (distanceTable[i][j] < 10)
                {
                    System.out.print("   ");
                }
                else if (distanceTable[i][j] < 100)
                {
                    System.out.print("  ");
                }
                else
                {
                    System.out.print(" ");
                }

                System.out.print(distanceTable[i][j]);
            }
            System.out.println();
        }
    }
}
