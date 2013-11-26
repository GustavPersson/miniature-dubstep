#include "LogicTestTrigger.h"

/**
 * Trigger Test 1
 * Tests trigger dependency. Uses the same requirement type for each one.
 * The trigger hierarchy is as following:
 *
 *				  Parent
 *		FirstChild		SecondChild
 * ChildOfFirst
 *
 * Parent will not activate until the others have (the dependency type is set
 * to false). FirstChild must wait for ChildOfFirst.
 *
 * The order of fullfilled requirements is as following:
 * Parent
 * FirstChild
 * ChildOfFirst
 * SecondChild
 *
 * Run from empty main().
 */
void testTrigger1()
{
	/*DEBUG_MESSAGE(LEVEL_LOW, "Trigger Test")

	EventManager *manager = myNew EventManager("main", true);
	TriggerHandler *handler = myNew TriggerHandler();

	//Parent trigger.
	TriggerBase *trigger1 = myNew TriggerEndMap();
	trigger1->setRequirement(TriggerRequirements::OBJECT_DESTROYED);
	trigger1->setSubRequirement(TriggerSubRequirements::OBJECT_ID);
	trigger1->setSubValue(5);
	trigger1->setTriggerName("Parent");
	handler->addTrigger(trigger1);

	//First child of parent trigger.
	TriggerBase *triggerChild = myNew TriggerEndMap();
	triggerChild->setRequirement(TriggerRequirements::OBJECT_DESTROYED);
	triggerChild->setSubRequirement(TriggerSubRequirements::OBJECT_ID);
	triggerChild->setSubValue(1);
	triggerChild->setTriggerName("FirstChild");
	handler->addTrigger(triggerChild);
	trigger1->addDependencyUnique(triggerChild, false);

	//Child of parent trigger's first child.
	TriggerBase *triggerChild2 = myNew TriggerEndMap();
	triggerChild2->setRequirement(TriggerRequirements::OBJECT_DESTROYED);
	triggerChild2->setSubRequirement(TriggerSubRequirements::OBJECT_ID);
	triggerChild2->setSubValue(0);
	triggerChild2->setTriggerName("ChildOfFirst");
	handler->addTrigger(triggerChild2);
	triggerChild->addDependencyUnique(triggerChild2, false);

	//Second child of parent trigger.
	TriggerBase *triggerChild3 = myNew TriggerEndMap();
	triggerChild3->setRequirement(TriggerRequirements::OBJECT_DESTROYED);
	triggerChild3->setSubRequirement(TriggerSubRequirements::OBJECT_ID);
	triggerChild3->setSubValue(4);
	triggerChild3->setTriggerName("SecondChild");
	handler->addTrigger(triggerChild3);
	trigger1->addDependencyUnique(triggerChild3, false);

	safeAddListener(handler->getEventListenerPointer(), EvtData_From_ActorDestroyed::msEventType);
	safeAddListener(handler->getEventListenerPointer(), EvtData_System_StartGame::msEventType);
	DEBUG_MESSAGE(LEVEL_LOW, "EventListener: " << handler->getName())

	//Send event to fullfill the requirements of Parent.
	safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(5)));
	manager->tick(10);

	Sleep(3000);

	//Send event to fullfill the requirements of FirstChild.
	safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(1)));
	manager->tick(10);

	Sleep(3000);

	//Send event to fullfill the requirements of ChildOfFirst.
	safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(0)));
	manager->tick(10);

	Sleep(3000);

	//Send event to fullfill the requirements of SecondChild.
	safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(4)));
	manager->tick(10);

	Sleep(3000);

	DEBUG_MESSAGE(LEVEL_LOWER, "Sending empty event...")
	safeQueueEvent(EventDataPtr(myNew EvtData_System_StartGame(0.0f)));
	manager->tick(10);

	delete manager;*/
}