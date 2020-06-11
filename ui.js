import EventEmitter from "events";

const formatQt = qt => {
  let processed = { ...qt };
  if (qt.style) {
    processed.style = JSON.stringify(qt.style || {});
  }
  return JSON.stringify(processed);
};

const mountUI = qt => {
  try {
    engine.mountUI(formatQt(qt));
  } catch (err) {}
  // console.log('mount ' + qt.widget);
};

const unmountUI = qt => {
  try {
    engine.unmountUI(formatQt(qt));
  } catch (err) {}
  // console.log('unmount ' + qt.widget);
};

const updateUI = qt => {
  try {
    engine.updateUI(formatQt(qt));
  } catch (err) {}
  console.log("update " + qt.id);
};

const events = {};

export { mountUI, unmountUI, updateUI, events };

window.$events = events;
