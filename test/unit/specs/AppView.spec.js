import Vue from 'vue'
import Hello from 'src/components/Hello'

describe('AppView', () => {
    it('should render correct contents', () => {
        const vm = new Vue({
            el: document.createElement('div'),
            render: (h) => h(Hello)
        })
        expect(vm.$el.querySelector('.hello h1').textContent)
            .to.equal('Welcome to Your Vue.js App')
    })

    it('should autocomplete clients', (done) => {
        const vm = new Vue({
/*
            el: document.createElement('div'),
            render: (h) => h(Hello)
*/
        })

        vm.autocomplete_search({term:'789'}, (data)=>{
            expect(data).
        })

        expect(vm.$el.querySelector('.hello h1').textContent)
            .to.equal('Welcome to Your Vue.js App')
    })
})
