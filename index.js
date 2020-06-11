import React from "react";
import ReactDOM from "react-dom";

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
    text1: "abc",
    text2: "123",
    text3: "edf"
  });

  const testClicked = () => {
    setState({
      ...state,
      text1: "xxx"
    });
    console.log("click!");
  };

  const lineChanged = evt => {
    console.log(evt);
    setState({
      ...state,
      text1: evt.target.value
    });
  };

  const buttonClicked = evt => {
    console.log("clicked");
    try {
      engine.showInspector(false);
    } catch (err) {}
  };

  const onExit = evt => {
    console.log(evt);
  };

  return (
    <QWidget id="mainWindow">
      <QMenuBar id="mainMenu">
        <QMenu text="File">
          <QAction text="New"></QAction>
          <QAction text="Save"></QAction>
          <QAction text="Exit" onClick={onExit}></QAction>
        </QMenu>
        <QAction text="View"></QAction>
        <QAction text="Help"></QAction>
      </QMenuBar>
      <h1>Hello, world!</h1>
      <style dangerouslySetInnerHTML={{ __html: style }}></style>
      <QLineEdit
        text={state.text1}
        onChange={lineChanged}
        style={{ color: "red" }}
      />
      <QLabel text="<b>Hello</b> world" />
      <QHBoxLayout>
        <QPushButton id="mainButton" text={state.text2} onClick={buttonClicked}>
          {state.text2}
        </QPushButton>
        <QPushButton text={state.text3} onClick={buttonClicked}>
          {state.text3}
        </QPushButton>

        <QPushButton
          onClick={buttonClicked}
          style={{ height: "60px" }}
          className="specialButton"
        >
          <QHBoxLayout>
            <QLabel text={state.text1} />
          </QHBoxLayout>
        </QPushButton>
      </QHBoxLayout>
      <input
        value={state.text1}
        onChange={evt => {
          setState({ ...state, text1: evt.target.value });
        }}
      />
      <button onClick={testClicked}>test</button>
    </QWidget>
  );
};

ReactDOM.render(<App />, document.getElementById("root"));