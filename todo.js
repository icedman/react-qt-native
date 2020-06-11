import React from "react";
import ReactDOM from "react-dom";
import { v4 as uuid } from 'uuid';

import {
  QWidget,
  QPushButton,
  QLineEdit,
  QLabel,
  QHBoxLayout,
  QVBoxLayout,
  QMenuBar,
  QMenu,
  QAction
} from "./widgets";

const style =
  " \
.qt { \
  border: 1px solid #c0c0c0; \
  padding: 4px; \
} \
.QHBoxLayout { \
    display: flex; \
} \
.QVBoxLayout { \
    display: flex; \
} \
";

const App = () => {
  const [state, setState] = React.useState({
    todos: []
  });

  const addTodo = (evt) => {
    setState({
      ...state,
      newTodo: '',
      todos: [ ...state.todos, {
        id: uuid(),
        text: state.newTodo,
        checked: false
      }]
    })
  };

  const deleteTodo = (todo) => {
    let idx = state.todos.findIndex((t) => {
      return t.id === todo.id;
    });

    if (idx !== -1) {
      let updateTodos = [ ...state.todos ];
      updateTodos.splice(idx, 1);
      setState({
        ...state,
        todos: updateTodos
      })
    }
  }

  const todosRendered = state.todos.map((todo,idx) => {
    return <QHBoxLayout key={`todo-${idx}`}>
        <QLabel text={todo.text}/>
        <QPushButton text='delete' onClick={(evt) =>{ deleteTodo(todo) }}/>
      </QHBoxLayout>
  })

  return (
    <QWidget id="mainWindow">
    <style dangerouslySetInnerHTML={{ __html: style }}></style>
    <QLabel text="<h1>Todo</h1>"/>
    <QLineEdit text={state.newTodo}
      onChange={(evt)=>{ setState({...state, newTodo: evt.target.value }) }}
      onSubmit={(evt)=>{ addTodo(evt); console.log(123); }}/>
    <QPushButton text="Add Todo" onClick={addTodo}/>
    {todosRendered}
    <button onClick={addTodo}>Add Todo</button>
    <code>{JSON.stringify(state.todos,null,4)}</code>
    </QWidget>
  );
};

ReactDOM.render(<App />, document.getElementById("root"));
