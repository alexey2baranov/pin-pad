import Vue from 'vue'
import MuseUI from 'muse-ui'
import 'muse-ui/dist/muse-ui.css'
import 'muse-ui/dist/theme-dark.css' // use dark theme


// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
require('jquery-ui/themes/base/core.css');
require('jquery-ui/themes/base/menu.css');
require('jquery-ui/themes/base/autocomplete.css');
require('jquery-ui/themes/base/theme.css');

import AppView from './components/AppView.vue'
import App from './App'

window.app= new App();

Vue.use(MuseUI)

let appView = new Vue({
    el: '#app',
    data: {
        app
    },
    template: '<app-view v-bind:model="app"/>',
    components: {AppView}
})


//fetch("http://217.114.191.210/sadfsadfsadf")
