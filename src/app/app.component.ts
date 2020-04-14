import { Component, OnInit } from "@angular/core";
import { HttpClient } from "@angular/common/http";

@Component({
  selector: "app-root",
  templateUrl: "./app.component.html",
  styleUrls: ["./app.component.css"],
})
export class AppComponent implements OnInit {
  public tableData: any;
  public proximityAmount: number;
  public waterTapAmount: number;
  constructor(private http: HttpClient) {}

  ngOnInit() {
    this.getData();
    setInterval(() => {
      this.getData();
    }, 5000);
  }

  getData() {
    this.http
      .get("https://timetowashyourhands.herokuapp.com/getData")
      .subscribe((data) => {
        this.tableData = data;
        this.proximityAmount = this.getProximityAmount(data);
        this.waterTapAmount = this.getWaterTapAmount(data);
        console.log(this.tableData);
      });
  }
  getProximityAmount(table): number {
    let filteredTable = table.filter((element) => {
      return element[3] === "occupied";
    });
    return filteredTable.length;
  }
  getWaterTapAmount(table): number {
    let filteredTable = table.filter((element) => {
      return element[1] == 1 && element[3] === "occupied";
    });
    return filteredTable.length;
  }

  turnOnSprayer() {
    this.http
      .get("https://timetowashyourhands.herokuapp.com/spray")
      .subscribe((data) => {
        window.alert("Sprayer turned on");
        console.log(data);
      });
  }
}
