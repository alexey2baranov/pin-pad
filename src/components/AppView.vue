<template>
    <div id="app">
        <div id="error" v-if="unhandledRejection">
            Ошибка <br>
            {{unhandledRejection.reason.message}}<br>
            <!--
                        <pre> стек в продакшене ужасный!!!
                            {{unhandledRejection.reason.stack}}
                        </pre>
            -->
        </div>
        <form @submit.prevent="1">
            <input type="text" id="search" placeholder="Номер договора или ФИО (Esc)">
            <mu-auto-complete id="search2" labelFloat label="Номер договора или ФИО (Esc)" fullWidth
                              :dataSource="autocomplete_dataSource" filter="noFilter"/>


            <subscriber-view v-if="model.subscriber" v-bind:model="model.subscriber"
                             @accountNumberChange="subscriberView_accountNumberChange"></subscriber-view>


            <template v-if="model.subscriber">
                <input type="number" id="sum" min="0" placeholder="Сумма оплаты" v-model.number="model.sum">
                <input type="submit" id="pay" :disabled="!(model.account && model.sum && model.sberbankServerStatus)"
                       value="Оплатить (Enter)"
                       @click="model.pay.bind(model)()">
                <div id="sberbankServerStatusMessage" v-if="!model.sberbankServerStatus">Платежный сервис ПАО "Сбербанк"
                    не доступен
                </div>
            </template>
            <pre>
                    <!--{{ model.account}}-->
                <!--{{model.subscriber}}-->
                    </pre>
        </form>
    </div>
</template>

<script>
    import $ from 'jquery'
    import autocomplete from 'jquery-ui/ui/widgets/autocomplete'
    import debounce from 'lodash/debounce'
    import Rx from 'rxjs/Rx'
    //    let Rx = require('@reactivex/rxjs')

    import SubscriberView from './SubscriberView'

    let config = require('../../config/main')[process.env.NODE_ENV];
    console.log("config", config);


    export default {
        name: 'app-view',
        data: function () {
            return {
                unhandledRejection: null,
                autocomplete_dataSource: [{text: "javascript", value: 1}]
            }
        },
        props: ["model"],
        components: {
            SubscriberView
        },
        computed: {},
        methods: {
            /*            autocomplete_input(search){
             console.log(search)
             this.$data.autocomplete_dataSource = []
             this.debouncedSetAutocompleteDatasource(search)
             },*/

            /**
             * Ищет клиентов удовлетворяющих поиску
             * Преобразовывает массив объектов получаемый по http в правильный массив у которого
             * text- это отображаемое имя клиента, а value- идентификатор клиента
             *
             * @return <[{text, value}]> Promise
             */
            getSubscribers(term){
                let url = new URL(`http://217.114.191.210/hd/public/Billing_SearcherView_Callback.php`)
                url.searchParams.append("term", term)
                return fetch(url)
                    .then(response => response.json())
                    .then(result => {
                        let fixedResult = result.map((eachResult) => {
                            return {
                                text: eachResult.label,
                                value: eachResult.value,
                            }
                        })
                        console.log("search result", fixedResult)
                        return fixedResult
                    })
                    .catch((err) => {
                        throw new Error(`Autocomplete search Error ${url.href}: ${err}`)
                    })
            },

            /**
             * выбирает текущим клиентом того, кого оператор выбрал в автокомплите
             * и ресет формы reset()
             * @param event
             * @param ui
             */
            autocomplete_select(event, ui) {
                this.model.getSubscriber(ui.item.id)
                    .then(subscriber => {
                        this.model.subscriber = subscriber;
//                        return fetch("http://217.114.191.210/sadfsadfsadf")
                    })
                    .catch(err => {
                        throw new Error(`Autocomplete select Error: ${err}`)
                    });
                document.forms[0].reset()
            },

            subscriberView_accountNumberChange(value){
                console.log("account changed", value);
                this.model.account = value;
            },
        },
        beforeCreate(){
            window.addEventListener('unhandledrejection', (event) => {
                this.$data.unhandledRejection = event;
            });
        },
        mounted() {
            Rx.Observable.fromEvent(document.querySelector("#search2 input"), "input")
                .do(() => {
                    this.$data.autocomplete_dataSource = []
                })
                .debounceTime(300)

                .subscribe(event => {
                    this.getSubscribers(event.target.value)
                        .then(subscribers => {
                            this.$data.autocomplete_dataSource = subscribers
                        })
                })
        }
    }
</script>

<style>
    * {
        font-family: 'Avenir', Helvetica, Arial, sans-serif;
        -webkit-font-smoothing: antialiased;
        -moz-osx-font-smoothing: grayscale;
        font-size: 20px;
    }

    #search {
        width: 20em;

        display: block;
        font-size: 1.5em;
    }

    #search2 input {
        font-size: 1.5em;
    }

    #sum {
        width: 10em;

        font-size: 1.5em;
        margin-top: 2em;
    }

    #pay {
        font-size: 1.5em;
        /*background-color: green;*/
        /*color: white;*/
    }

    #error {
        background-color: darkred;
        color: white;
        margin: 1em 0;
    }

    #sberbankServerStatusMessage {
        background-color: darkred;
        color: white;
    }

</style>
