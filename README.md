[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Jwdk2u8p)
# cosc2804-assignment3-template
Team Member Allocations:
- Alex: Maze Solving, E1 (Build Maze Without Flattening Terrain)
- Caiden: Maze Building & Cleaning World, E2 (Find Shortest Path)
- Jonas: Maze Generation, E3 (Validate User Input Maze)

Compiling and Running:
- Run ```make && ./mazeRunner``` in a unix terminal

Assumptions:
- Solving Manually: Solving manually can only occur after a maze has been generated and built. The random location within a maze is chosen via the set of all air blocks in one y-coord level within a maze, rather than the set of all blocks within a maze in one y-coord level. Meaning the algorithm does not just loop naively until a random air block is found.
- Show Escape Route: Showing the escape route can only occur after a maze has been generated and built. The blue carpet marking the exit is one block out of the maze (where an edge of a maze could go). Also assume that the lime carpet can be oriented in any way, such 
that the 'player's right hand' is facing a wall.
