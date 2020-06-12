import React from "react";
import clsx from "clsx";
import { v4 as uuid } from "uuid";
import { mountUI, unmountUI, updateUI, events } from "./ui";

const QWidget_ = props => {
  const [qt, setQt] = React.useState({
    widget: props.widget || "QWidget",
    id: props.id || uuid(),
    parent: props.parent
  });

  let uiInfo = { ...props, ...qt, className: className };

  React.useEffect(() => {
    mountUI(uiInfo);
    return () => {
      unmountUI(uiInfo);
    };
  }, []);

  let className = clsx("qt", props.widget, props.className);
  updateUI(uiInfo);

  let children = [];
  if (typeof props.children === "object") {
    {
      React.Children.map(props.children, (c, idx) => {
        if (!React.isValidElement(c)) {
          children.push(c);
          return;
        }
        children.push(
          React.cloneElement(
            c,
            { ...c.props, parent: qt.id, key: `${qt.id}-${idx}` },
            c.props.children
          )
        );
      });
    }
  }

  // preview on html only
  return (
    <div {...qt} className={className} style={props.style}>
      {qt.widget}::{qt.id} {children}
    </div>
  );
};

const QWidget = QWidget_;
// const QWidget = React.memo(QWidget_);

const QPushButton = props => {
  return (
    <QWidget {...props} widget="QPushButton">
      {props.text} {props.children}
    </QWidget>
  );
};

const QLineEdit = props => {
  return (
    <QWidget {...props} widget="QLineEdit">
      {props.text}
    </QWidget>
  );
};

const QLabel = props => {
  return (
    <QWidget {...props} widget="QLabel">
      {props.text}
    </QWidget>
  );
};

const QHBoxLayout = props => {
  return (
    <QWidget {...props} widget="QHBoxLayout">
      {props.children}
    </QWidget>
  );
};

const QVBoxLayout = props => {
  return (
    <QWidget {...props} widget="QVBoxLayout">
      {props.children}
    </QWidget>
  );
};

const QMenuBar = props => {
  return (
    <QWidget {...props} widget="QMenuBar">
      {props.children}
    </QWidget>
  );
};

const QMenu = props => {
  return (
    <QWidget {...props} widget="QMenu" child={true}>
      {props.children}
    </QWidget>
  );
};

const QAction = props => {
  return (
    <QWidget {...props} widget="QAction" child={true}>
      {props.text}
    </QWidget>
  );
};

export {
  QWidget,
  QPushButton,
  QLineEdit,
  QLabel,
  QHBoxLayout,
  QVBoxLayout,
  QMenuBar,
  QMenu,
  QAction
};
