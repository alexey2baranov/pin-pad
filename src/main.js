import Vue from 'vue'
import MuseUI from 'muse-ui'
import 'muse-ui/dist/muse-ui.css'
// import 'muse-ui/dist/theme-dark.css' // use dark theme

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
