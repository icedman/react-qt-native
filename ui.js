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
  const _events = [ 'onChange', 'onClick', 'onSubmit' ];

  try {

  events[qt.id] = events[qt.id] || {};
  _events.forEach(e => {
    events[qt.id][e] = qt[e] || (evt => {});
  })
    engine.updateUI(formatQt(qt));
  } catch (err) {}
  // console.log("update " + qt.id);
};

window.$events = events;

export { mountUI, unmountUI, updateUI, events };
