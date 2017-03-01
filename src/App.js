/**
 * Created by alexey2baranov on 14.01.17.
 */
import $ from 'jquery'

let config = require('../config/main')[process.env.NODE_ENV];

export default class App {
    constructor() {
        this.subscriber = null
        this.sum = null
        this.account = null
        this.cheq = null

        /**
         * онлайн статус сервера сбербанка
         * и обновление статуса
         */
        this.sberbankServerStatus = true
        if (!navigator.userAgent.match(/linux/i)) {
            setInterval(() => {
                let url = new URL("http://localhost:3000/ping");
                return fetch(url)
                    .then(response => this.sberbankServerStatus = true)
                    .catch(err => {
                        this.sberbankServerStatus = false;
                    });
            }, 5000)
        }
    }

    /**
     * Возвращает клиента по id
     */
    getSubscriber(id) {
        let url = new URL(`${config.server.schema}://${config.server.host}:${config.server.port}/${config.server.path}/getSubscriber`);
        url.searchParams.append("id", id);
        return fetch(url)
            .then(response => response.json())
            .then(subscriber => {
                console.log("subscriber loaded", subscriber);
                return subscriber;
            })
            .catch(err => {
                throw new Error(`loading subscriber error ${url.href}: ${err}`)
            })
    }

    /**
     * Оплатил и сумму = 0
     */
    pay() {
        this.payToSberbank()
            .then(() => this.payToBilling())
            .then(() => {
                console.log("updating subscriber")
                return this.getSubscriber(this.subscriber.id)
            })
            .then(subscriber => {
                this.subscriber = subscriber
                this.sum = 0 //это нельзя делать синхронно потому что sum используется в асинхронных вызовах payToSberbank и payToBilling
            })
            .catch(err => {
                throw err;
            })
    }

    payToSberbank() {
        let url = new URL("http://localhost:3000/");
        url.searchParams.append("operation", 1)
        url.searchParams.append("sum", this.sum)
        return fetch(url)
            .then(response => response.json())
            .then(result => {
                console.log("sberbank call result", result)
                if (result.status != "success") {
                    throw new Error(result.error.message);
                }
                else{
                    this.cheq= result.cheq
                }
            })
            .catch(function (err) {
                throw new Error(`sberbank Error ${url.href}: ${err}`)
            })
    }

    payToBilling() {
        let url = new URL(config.weboplata.url);
        url.searchParams.append("method", "add");
        url.searchParams.append("AccountNumber", this.account.number);
        url.searchParams.append("ServiceType", 1);
        url.searchParams.append("PaymentSumm", this.sum);
        url.searchParams.append("PaymentDate", Math.round(new Date().getTime() / 1000));
        url.searchParams.append("PSID", "reception");
        url.searchParams.append("ExternalPaymentID", Math.round(new Date().getTime() / 1000));

        return fetch(url)
            .then(response => response.text())
            .then(result => {
                console.log("weboplata call result", result)
                if ($("status", result).text() == "failed") {
                    throw new Error($("message", result).text())
                }
            })
            .catch(function (err) {
                throw new Error(`Weboplata call Error ${url.href}: ${err}`)
            });
    }
}
