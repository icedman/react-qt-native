import EventEmitter from "events";

const events = {};

const formatQt = qt => {
  let processed = { ...qt };
  if (qt.style) {
    processed.style = JSON.stringify(qt.style || {});
  }
  delete processed.children;
  return JSON.stringify(processed);
};

const mountUI = qt => {
  try {
    // engine.mountUI(formatQt(qt));
  } catch (err) {}
  console.log('mount ' + qt.widget);
};

const unmountUI = qt => {
  try {
    engine.unmountUI(formatQt(qt));
    delete events[qt.id];
  } catch (err) {}
  // console.log('unmount ' + qt.widget);
};

const updateUI = qt => {
  try {

  events[qt.id] = events[qt.id] || {}
  events[qt.id].onChange = qt.onChange || (evt => {});
  events[qt.id].onClick = qt.onClick || (evt => {});
  events[qt.id].onSubmit = qt.onSubmit || (evt => {});

    engine.updateUI(formatQt(qt));
  } catch (err) {}
  // console.log("update " + qt.id);
};

window.$events = events;

export { mountUI, unmountUI, updateUI, events };
