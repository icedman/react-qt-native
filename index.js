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
    flex-direction: row; \
} \
.QVBoxLayout { \
    display: flex; \
    flex-direction: column; \
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
    try {
      engine.exit();
    } catch (err) {}
  };

  const submitted = evt => {
    console.log("submitted!");
  };

  return (
    <QWidget id="mainWindow">
      <style dangerouslySetInnerHTML={{ __html: style }}></style>
      <QMenuBar id="menuBar">
        <QMenu text="&File">
          <QAction text="New"></QAction>
          <QAction text="&Open" shortcut="Ctrl+O"></QAction>
          <QAction text="&Save"></QAction>
          <QAction text="E&xit" onClick={onExit}></QAction>
        </QMenu>
        <QAction text="View"></QAction>
        <QAction text="Help"></QAction>
      </QMenuBar>
      <h1>Hello, world!</h1>
      <QLineEdit
        text={state.text1}
        onChange={lineChanged}
        onSubmit={submitted}
        style={{ color: "red" }}
      />
      <QLabel text="<b>Hello</b> <i>world</i>" />
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
