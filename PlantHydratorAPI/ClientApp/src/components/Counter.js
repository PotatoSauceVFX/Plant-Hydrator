import React, { Component } from 'react';

export class Counter extends Component {
    static displayName = Counter.name;

    constructor(props) {
        super(props);
        this.state = {
            count: 0,
            loading: true
        };
        this.incrementCounter = this.incrementCounter.bind(this);
        this.decrementCounter = this.decrementCounter.bind(this);
        this.apiIncrementCount = this.apiIncrementCount.bind(this);
        this.apiDecrementCount = this.apiDecrementCount.bind(this);
    }

    componentDidMount() {
        this.getCount();
    }

    incrementCounter() {
        this.apiIncrementCount();
    }

    decrementCounter() {
        this.apiDecrementCount();
    }

    render() {
        return (
            <div>
                <h1>Counter</h1>
                <p>This is a simple example of a React component.</p>

                <p aria-live="polite">Count: <strong>{this.state.count}</strong></p>

                <button className="btn btn-primary mr-3" disabled={this.state.loading} onClick={this.incrementCounter}>Increment</button>
                <button className="btn btn-danger" disabled={this.state.loading} onClick={this.decrementCounter}>Decrement</button>
            </div>
        );
    }


    apiIncrementCount() {
        this.setState({ loading: true }, () => {
            fetch('api/count/increment', { method: "post" })
                .then(res => res.json())
                .then((result) => {
                    console.log("Data: ", result);
                    this.setState({ count: result.Count, loading: false });
                }, (error) => {
                    console.error("Error", error);
                });
        });
    }

    apiDecrementCount() {
        this.setState({ loading: true }, () => {
            fetch('api/count/decrement', { method: "post" })
                .then(res => res.json())
                .then((result) => {
                    console.log("Data: ", result);
                    this.setState({ count: result.Count, loading: false });
                }, (error) => {
                    console.error("Error", error);
                });
        });
    }


    async getCount() {
        await fetch('api/count')
            .then(res => res.json())
            .then((res) => {
                this.setState({ count: res.Count, loading: false });
            });

    }
}
