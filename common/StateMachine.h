/* State Class
 * Created by ASH for EEE2041
 * Handels the Game States
 */
#ifndef StateMachine_H_
#define StateMachine_H_

#include <GL/glew.h>
#include <GL/glut.h>

//! Virtual State Class
class State
{
public:
  // Constructor
  State();
  // Destructor
  virtual ~State();

  // The different 'events' functions. Child classes can 
  // implement the ones in which they are interested in.
  virtual void HandelKeys( ) { }
  virtual void HandelKeyStates( )   { }
  virtual void Update( )  { } //! Physics and Updates
  virtual void Draw()  { }  //! Rendering

  // Functions called when the state is entered or exited
  // (transition from/to another state).
  virtual void EnterState()  { } //! Init
  virtual void ExitState()   { } //! Exit   

protected:
  // Helper function to switch to a new active state.
  void ChangeState(State* pNewState);
};

//! Menu State
class MenuState : public State
{
public:
    // Constructor
    MenuState();
    // Destructor
    ~MenuState();
};

//! Menu State
class GameState : public State
{
public:
    // Constructor
    GameState();
    // Destructor
    ~GameState();

    // The different 'events' functions. Child classes can 
    // implement the ones in which they are interested in.
    void HandelKeys( ) { }
    void HandelKeyStates( )   { }
    void Update( )  { } //! Physics and Updates
    void Draw()  { }  //! Rendering

  // Functions called when the state is entered or exited
  // (transition from/to another state).
    void EnterState()  { } //! Init
    void ExitState()   { } //! Exit   
};

#endif

