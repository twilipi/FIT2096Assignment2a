/*
{
//check the player is touching the wall or not, by checking its direction
//(although it could be a little buggy on the detection, at least it works first,
//but it's best to find a matrix way but that's way too hard for me in certain time limit)
//known bug: when going to the corner of some wall pattern, it can sneak inside easily, if it's over the bound, it'll crash the program.

//if goes up
if (m_mapBlocks[arrZ+1][arrX]->getAttr() == "wall") {
m_player->SetPosition(Vector3(posX, 0.0f, posZ-1.0f));
}


//if goes down
if (posZ > -(static_cast<float>(m_mapSize) / 2 + 0.25f))
{
int predictDown = static_cast<int>(correctedPosZ - 0.25f);
//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
if ((correctedPosZ - 0.25f) > 0) { predictDown += 11; }
else { predictDown += 10; }
//trace the predicted array index is wall or not
if (m_mapBlocks[predictDown][arrX]->getAttr() == "wall") {
// if yes, adjust it back to the normal position
float adjustZ = static_cast<float>(gridZ) - 0.25f;
m_player->SetPosition(Vector3(posX, 0.0f, adjustZ));
}
}
else { m_player->SetPosition(Vector3(posX, 0.0f, -(static_cast<float>(m_mapSize) / 2 + 0.25f))); }


//if goes left
if (posX > -(static_cast<float>(m_mapSize) / 2 + 0.25f))
{
int predictLeft = static_cast<int>(correctedPosX - 0.25f);
//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
if ((correctedPosX - 0.25f) > 0) { predictLeft += 11; }
else { predictLeft += 10; }
//trace the predicted array index is wall or not
if (m_mapBlocks[arrZ][predictLeft]->getAttr() == "wall") {
// if yes, adjust it back to the normal position
float adjustX = static_cast<float>(gridX) - 0.25f;
m_player->SetPosition(Vector3(adjustX, 0.0f, posZ));
}
}
else { m_player->SetPosition(Vector3(-(static_cast<float>(m_mapSize) / 2 + 0.25f), 0.0f, posZ)); }


//if goes right
if (posX < (static_cast<float>(m_mapSize) / 2 - 0.75f))
{
int predictRight = static_cast<int>(correctedPosX + 0.25f);
//de-double zero (super dirty way, really need someone how to use a proper way to get the colision mechanism)
if ((correctedPosX + 0.25f) > 0) { predictRight += 11; }
else { predictRight += 10; }
//trace the predicted array index is wall or not
if (m_mapBlocks[arrZ][predictRight]->getAttr() == "wall") {
// if yes, adjust it back to the normal position
float adjustX = static_cast<float>(gridX) + 0.25f;
m_player->SetPosition(Vector3(adjustX, 0.0f, posZ));
}
}
else { m_player->SetPosition(Vector3((static_cast<float>(m_mapSize) / 2 - 0.75f), 0.0f, posZ)); }


}

*/


/*
//clear screen and display the current status of player and enemy
system("cls");
cout << b_enemy->displayStatus() << "\n\n\t\t\tVS\n\n" << b_player->displayStatus() << "\nAuto Battle: please wait until someone is defeated\n";
Sleep(500);
cout << "it's " << battleRotate[battleCounter]->getName() << "'s turn!\n";
//if the index is a player, ask the player for the command
*/